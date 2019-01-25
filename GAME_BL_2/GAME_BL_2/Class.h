#pragma once

class Gamer // класс с параметрами игрока
{
	// private поля класса
	CHAR login[255]; // логин игрока
	CHAR password[255]; // пароль игрока
	BOOL remember; // введет ли автоматически данные игрока при входе
	unsigned long long record; // рекорд игрока

public:
	Gamer() {} // конструктор без параметров
	Gamer(CHAR * login, CHAR * password, unsigned long long record) // конструктор для инициализации параметров
	{
		strcpy(this->login, login); // инициализация логина
		strcpy(this->password, password); // инициализация пароля
		remember = FALSE; // инициализация "запоминания"
		this->record = record; // инициализация рекорда
	}

	void set_all(CHAR * login, CHAR * password, BOOL remember, unsigned long long record) // функция для инициализации параметров
	{
		strcpy(this->login, login); // инициализация логина
		strcpy(this->password, password); // инициализация пароля
		this->remember = remember; // инициализация "запоминания"
		this->record = record; // инициализация рекорда
	}

	/***** GET ФУНКЦИИ *****/
	CHAR * get_login() { return login; } // получения логина
	CHAR * get_password() { return password; } // получение пароля
	BOOL get_remember() { return remember; } // получение "запоминания"
	unsigned long long get_record() { return record; } // получение рекорда

	/***** SET ФУНКЦИИ *****/
	void set_login(CHAR * login) { strcpy(this->login, login); } // установка логина
	void set_password(CHAR * password) { strcpy(this->password, password); } // установка пароля
	void set_remember(BOOL remember) { this->remember = remember; } // установка "запоминания"
	void set_record(unsigned long long record) { this->record = record; } // установка рекорда
};