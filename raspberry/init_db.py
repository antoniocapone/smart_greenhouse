import sqlite3

def main() -> None:
	conn = sqlite3.connect("progetto_tiai.db")
	conn.execute("""
		CREATE TABLE MISURAZIONI(
			id INTEGER PRIMARY KEY AUTOINCREMENT,
			tipo VARCHAR(20) NOT NULL,
			valore INTEGER NOT NULL,
			data INTEGER NOT NULL
		);
	""")
	conn.close()

if __name__ == "__main__":
	main()