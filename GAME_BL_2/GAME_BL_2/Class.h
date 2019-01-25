#pragma once

class Gamer // ����� � ����������� ������
{
	// private ���� ������
	CHAR login[255]; // ����� ������
	CHAR password[255]; // ������ ������
	BOOL remember; // ������ �� ������������� ������ ������ ��� �����
	unsigned long long record; // ������ ������

public:
	Gamer() {} // ����������� ��� ����������
	Gamer(CHAR * login, CHAR * password, unsigned long long record) // ����������� ��� ������������� ����������
	{
		strcpy(this->login, login); // ������������� ������
		strcpy(this->password, password); // ������������� ������
		remember = FALSE; // ������������� "�����������"
		this->record = record; // ������������� �������
	}

	void set_all(CHAR * login, CHAR * password, BOOL remember, unsigned long long record) // ������� ��� ������������� ����������
	{
		strcpy(this->login, login); // ������������� ������
		strcpy(this->password, password); // ������������� ������
		this->remember = remember; // ������������� "�����������"
		this->record = record; // ������������� �������
	}

	/***** GET ������� *****/
	CHAR * get_login() { return login; } // ��������� ������
	CHAR * get_password() { return password; } // ��������� ������
	BOOL get_remember() { return remember; } // ��������� "�����������"
	unsigned long long get_record() { return record; } // ��������� �������

	/***** SET ������� *****/
	void set_login(CHAR * login) { strcpy(this->login, login); } // ��������� ������
	void set_password(CHAR * password) { strcpy(this->password, password); } // ��������� ������
	void set_remember(BOOL remember) { this->remember = remember; } // ��������� "�����������"
	void set_record(unsigned long long record) { this->record = record; } // ��������� �������
};