#include "main.h"

int initializeDb(MYSQL *connection) {
	int rv = 0;// mysql_query(connection, "CREATE DATABASE quotedb");
	if (!rv) {
		mysql_query(connection, "DROP TABLE IF EXISTS datatable");
		return mysql_query(connection, "CREATE TABLE datatable(symbol TEXT, low DOUBLE, high DOUBLE, open DOUBLE, close DOUBLE, time BIGINT)");
	}
	return -1;
}

crs_string constructURL(const crs_string forexUrl, const crs_string forexApiKey, const crs_string &firstCurrency, const crs_string &secondCurrency) {
	return forexUrl + U("&from_symbol=") + firstCurrency + U("&to_symbol=") + secondCurrency + U("&interval=1min&apikey=") + forexApiKey;
}

//@platformspecific
crs_string getPassword() {
	std::cout << "Password: ";

	HANDLE hStdin = GetStdHandle(STD_INPUT_HANDLE);
	DWORD mode;
	GetConsoleMode(hStdin, &mode);
	//disable echoing only for password input
	mode &= ~ENABLE_ECHO_INPUT;
	SetConsoleMode(hStdin, mode);

	crs_string password;
	std::wcin >> password;
	
	mode |= ENABLE_ECHO_INPUT;
	SetConsoleMode(hStdin, mode);

	std::cout << "\n";

	return password;
}

//@platformspecific
bool saveCredential(const crs_string &user, const crs_string &pass) {
	bool rv = false;

	CREDENTIALW cred;
	memset(&cred, 0, sizeof(cred));

	cred.TargetName = (LPWSTR)CRED_NAME;
	cred.Type = CRED_TYPE_GENERIC;
	cred.Persist = CRED_PERSIST_LOCAL_MACHINE;

	wchar_t userName[CRED_MAX_USERNAME_LENGTH];
	cred.UserName = userName;
	wcscpy_s(cred.UserName, CRED_MAX_USERNAME_LENGTH, user.c_str());

	BYTE blob[CRED_MAX_CREDENTIAL_BLOB_SIZE];
	cred.CredentialBlob = blob;
	size_t len;
	wcstombs_s(&len, (char*)cred.CredentialBlob, CRED_MAX_CREDENTIAL_BLOB_SIZE, pass.c_str(), CRED_MAX_CREDENTIAL_BLOB_SIZE - 1);
	cred.CredentialBlobSize = strlen((const char*)cred.CredentialBlob);

	if (CredWriteW(&cred, NULL)) {
		rv = true;
	} else {
		DWORD err = GetLastError();
		switch (err) {
		case ERROR_INVALID_PARAMETER: std::cout << "Invalid parameter\n";
		case ERROR_BAD_USERNAME: std::cout << "Bad username\n";
		default: std::cout << err << "\n";
		}
	}

	return rv;
}

/*
If we're given neither username nor password, look for cached credentials
If those are found, silently connect; otherwise, prompt user to enter them
If we're given a username, ask for password as well
This works for now when we only have one user

@platformspecific
*/
int main(int argc, char *argv[]) {
	crs_string forexUrl, forexApiKey, dbUrl, dbUsername, dbPassword;

	bool credentialsCached = false;

	PCREDENTIALW cred = nullptr;
	//retrieve creds only if given no input
	if (argc == 1) {
		credentialsCached = CredReadW(CRED_NAME, CRED_TYPE_GENERIC, NULL, &cred) ? true : false;
	}

	if (credentialsCached) {
		std::cout << "Using cached credentials\n";

		dbUsername = { cred->UserName };
		wchar_t pass[CRED_MAX_CREDENTIAL_BLOB_SIZE];
		size_t len;
		mbstowcs_s(&len, pass, (const char *)cred->CredentialBlob, CRED_MAX_CREDENTIAL_BLOB_SIZE);
		dbPassword = { pass };
	} else {
		if (argc == 1) {
			std::cout << "Username: ";
			std::wcin >> dbUsername;
			std::cout << "\n";
		} else {
			dbUsername = utility::conversions::to_string_t(argv[1]);
		}

		dbPassword = getPassword();

		if (saveCredential(dbUsername, dbPassword)) {
			std::cout << "New credentials saved\n";
		} else std::cout << "Using unsaved new credentials\n";
	}

	CredFree(cred);

	utility::ifstream_t in("connection_info.txt");
	in >> forexUrl >> forexApiKey >> dbUrl;
	DbInfo db(dbUrl, dbUsername, dbPassword);

	MYSQL *connection = mysql_init(NULL);
	if (connection) {
		mysql_ssl_set(connection, NULL, NULL, "rds-ca-2019-root.pem", NULL, NULL);
		connection = mysql_real_connect(connection, db.url, db.username, db.password, "quotedb", DB_PORT, NULL, 0);
	}

	int rv = -1;
	if (connection) {
		std::cout << "Connection established\n";
		rv = 0;// initializeDb(connection);
	}

	if (!rv) {
		//https://stackoverflow.com/questions/23310511/mysql-asynchronous
		//Every second, create a list of tasks to be run at increasing intervals in the future
		//Once all those tasks have returned, get the data from them and write it to the db
		std::vector<pplx::task<void>> taskList(FETCHES_PER_WRITE);

		std::mutex mut;

		//FIXME do actual error handling
		while (true) {
			try {
				std::vector<std::optional<DbData>> quotes(FETCHES_PER_WRITE);
				for (int i = 0; i < FETCHES_PER_WRITE; i++) {
					web::uri url(constructURL(forexUrl, forexApiKey, U("USD"), U("EUR")));
					taskList[i] = storeFromQuoteAfterDelay(connection, url, quotes[i], FETCH_INTERVAL * (i + 1));
				}
				auto fetchTasks = pplx::when_all(begin(taskList), end(taskList));
				fetchTasks.wait();

				//set the data that we're actually going to write
				DbData writeData(quotes[0].value());
				bool valid = true;
				for (const auto &quote : quotes) {
					if (quote.has_value()) {
						if (quote.value().low < writeData.low) {
							writeData.low = quote.value().low;
						}
						if (quote.value().high > writeData.high) {
							writeData.high = quote.value().high;
						}
					} else {
						valid = false;
						break;
					}
					
				}
				writeData.close = quotes[quotes.size() - 1].value().close;
				writeData.time = quotes[quotes.size() - 1].value().time;

				//make sure we have valid data
				if (!valid) {
					continue;
				}

				InsertionQuery iq(writeData);
				DeletionQuery dq(writeData.time);
				iq.execute(connection);
				dq.execute(connection);
			} catch (std::exception e) {
				std::cout << e.what() << "\n";
			} catch (...) {
				std::cout << "exception handled\n";
			}
		}
	}

	mysql_close(connection);
}