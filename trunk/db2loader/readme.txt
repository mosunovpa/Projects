
���������
=========

1. ���������� Jre 6.0.02. ����� ���������� �� jre1.6.0_02.rar � ����� C:\Program Files\Java.
2. ���� � ��� �� ���������� DB2 ������
	2.1. �������������� db2_jars.rar � C:\Program Files\Java.
	2.2. ��������� ��� � ���������� CLASSPATH ��������� ���� �� ��������� jar:
		CLASSPATH=
			.;
			C:\Program Files\java\db2java.zip;
			C:\Program Files\java\db2jcc.jar;
			C:\Program Files\java\sqlj.zip;
			C:\Program Files\java\db2jcc_license_cisuz.jar;
			C:\Program Files\java\db2jcc_license_cu.jar;
			C:\Program Files\java\common.jar;
			c:\Program Files\Java\jre1.6.0_02\lib\charsets.jar;

3. ���� ���������� DB2 ������
	2.1. ��������� ��� � ���������� CLASSPATH ��������� ���� �� ��������� jar:
		CLASSPATH=
			.;
			C:\PROGRA~1\IBM\SQLLIB\java\db2java.zip;
			C:\PROGRA~1\IBM\SQLLIB\java\db2jcc.jar;
			C:\PROGRA~1\IBM\SQLLIB\java\sqlj.zip;
			C:\PROGRA~1\IBM\SQLLIB\java\db2jcc_license_cisuz.jar;
			C:\PROGRA~1\IBM\SQLLIB\java\db2jcc_license_cu.jar;
			C:\PROGRA~1\IBM\SQLLIB\java\common.jar;
			c:\Program Files\Java\jre1.6.0_02\lib\charsets.jar;

4. ��������� ��� � ���������� PATH �������� ���� c:\Program Files\Java\jre1.6.0_02\bin.
5. ������� c:\WINDOWS\system32\java.exe �� ��������� ����������.
6. �������������� install.rar � ������� ���������.
7. �������� � ���������� CLASSPATH ������� ���������.

� ���������� ��������� ����� ������� ��������� ����� � ��������:

 |-bin\
 |  |-data\				- ������� ��� ����������� ������.
 |  |-log\				- ������� ����� ������ ����������.
 |  |-all.bat				- ��������� ���� ��� �������� ���� ���������� ������.
 |  |-bundling.bat			- �������� ������������ (������������� ��������� SBDSP057, SBDSP058, SBDSP059).
 |  |-cuts_passports.bat		- �������� ��������� ������� (������������� ��������� GSPNRP1B)
 |  |-delta_passports.bat		- �������� ��������� ��������� ���������� ������� (������������� ��������� GSPVAGS3)
 |  |-repair_passports.bat		- �������� ��������� �������� (������������� ��������� GSPNRP1B)
 |  |-wag_operations.bat		- �������� �������� � �������� (������������� ��������� GSPNRP1B)
 |- classes
 |  |-loader.class
 |  |-Data.class
 |  |-Db.class
 |  |-JdbcException.class
 |-readme.txt


������������� ����������
========================

��������� ��������� ������.
��������:
	java loader <alias> <host> <port> <user> <password> <parameters>, ���:
		alias - ����� ���� ������,
		host - ����� �������,
		port - ���� �������,
		user - ��� ������������,
		password - ������ ������������,
		parameters - ��������� ��������:
			-o<output file> - ��� ����� ��� �������� ������ (���� �� ������, ������ ����� ���������� � stdout).
			-l<log file> - ��� ����� ��� ��������� ��������� (���� �� ������, ������ ����� ���������� � stderr).
			-w<wagon file> - ��� ����� � �������� ������� �������.
			-h<number> - ���-�� ����� ��������� � �����, ��������� ���������� -w.
			-p<proc_name> - ������ ��� ��������� (<schema>.<procedure>).
			-t<result table name> - ��� ������� � �������������� ������� ������.
			-i<int_proc_param> - �������� �������� ���������.
			-s<string_proc_param> - ��������� �������� ���������. 
				������ ���������� ��������� ����� ���� ������� ������� %d � %y:
					%d[<n>(d|m|y)] - ������� ���� � ������� yyyy-MM-dd. 
						<n> - ���������� ���� (d), ������� (m), ��� (y) ����� �� ������� ����,
							��������, -s%d[1d] - ��������� ���� (������� ����� ���� ����)
					%y - ��� � ������� yyyy, �������� -s%y - ������� ���.

			-ri - �������� �������� �������� ���������.
			-rs - �������� ��������� �������� ���������.

�������:
	�������� ������ � ���� out.txt �� ��������� d90.gvcpst01 � 9-� �������� ����������� � ����� �������� ��������:
	java loader LOCP 10.200.8.1 5021 DESVOC01 DESV5066 -oout.txt -pd90.gvcpst01 -i2 -i3 -s(83000) -i0 -i4 -s() -i0 -s1 -s() -ri


�������� ���������� ������
==========================

��� �������� ���������� ������ ��������� ��������� ���� <�������_���������_����������>/bin/all.bat.
� ���������� � ������� <�������_���������_����������>/bin/data ����� ��������� ��������� �����:
	bundling_telega_<����>.csv	- ��������� ������������ ������� ��� ���������� �������.
	bundling_kolesa_<����>.csv	- ��������� ������������ �������� ��� ��� ���������� �������.
	bundling_lit_det_<����>.csv	- ��������� ������������ ����� ������� ��� ���������� �������.
	wag_operations_<����>.csv	- �������� � �������.
	repair_passports_<����>.csv	- �������� ��������.
	cuts_passports_<����>.csv	- �������� �������.
	delta_passports_<����>.csv	- ��������� � ��������� ���������� �������.
���� ������ ���������� ����������� � �������� <�������_���������_����������>/bin/log.
���� � ���������� �������� ������ ��������� ������, ���� ������ �� �����������. ������ �������������� � log-�����.
