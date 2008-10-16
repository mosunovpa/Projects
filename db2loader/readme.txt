
УСТАНОВКА
=========

1. Установите Jre 6.0.02. Можно развернуть из jre1.6.0_02.rar в папку C:\Program Files\Java.
2. Если у вас не установлен DB2 клиент
	2.1. Разархивируйте db2_jars.rar в C:\Program Files\Java.
	2.2. Убедитесь что в переменной CLASSPATH прописаны пути до следующих jar:
		CLASSPATH=
			.;
			C:\Program Files\java\db2java.zip;
			C:\Program Files\java\db2jcc.jar;
			C:\Program Files\java\sqlj.zip;
			C:\Program Files\java\db2jcc_license_cisuz.jar;
			C:\Program Files\java\db2jcc_license_cu.jar;
			C:\Program Files\java\common.jar;
			c:\Program Files\Java\jre1.6.0_02\lib\charsets.jar;

3. Если установлен DB2 клиент
	2.1. Убедитесь что в переменной CLASSPATH прописаны пути до следующих jar:
		CLASSPATH=
			.;
			C:\PROGRA~1\IBM\SQLLIB\java\db2java.zip;
			C:\PROGRA~1\IBM\SQLLIB\java\db2jcc.jar;
			C:\PROGRA~1\IBM\SQLLIB\java\sqlj.zip;
			C:\PROGRA~1\IBM\SQLLIB\java\db2jcc_license_cisuz.jar;
			C:\PROGRA~1\IBM\SQLLIB\java\db2jcc_license_cu.jar;
			C:\PROGRA~1\IBM\SQLLIB\java\common.jar;
			c:\Program Files\Java\jre1.6.0_02\lib\charsets.jar;

4. Убедитесь что в переменной PATH прописан путь c:\Program Files\Java\jre1.6.0_02\bin.
5. Удалите c:\WINDOWS\system32\java.exe во избежание конфликтов.
6. Скопируте классы из \\Ocrv-fukalov\pub\АнализКачестваРемонта\Данные\loader\v1\classes\ в каталог установки.
7. Добавьте в переменную CLASSPATH каталог установки.


ИСПОЛЬЗОВАНИЕ ЗАГРУЗЧИКА
========================

Запустите командную строку.
Наберите:
	java loader <alias> <host> <port> <user> <password> <parameters>, где:
		alias - алиас базы данных,
		host - адрес сервера,
		port - порт сервера,
		user - имя пользователя,
		password - пароль пользователя,
		parameters - параметры загрузки:
			-o<output file> - имя файля для выгрузки данных (если не указан, данные будут выводиться в stdout),
			-l<log file> - имя файля для сдужебных сообщений (если не указан, данные будут выводиться в stderr),
			-p<proc_name> - полное имя процедуры (<schema>.<procedure>),
			-i<int_proc_param> - числовой параметр процедуры,
			-s<string_proc_param> - строковый параметр процедуры,
			-ri - выходной числовой параметр процедуры,
			-rs - выходной строковый параметр процедуры.

Примеры:
	выгрузка данных в файл out.txt из процедуры d90.gvcpst01 с 9-ю входными параметрами и одним числовым выходным:
	java loader LOCP 10.200.8.1 5021 DESVOC01 DESV5066 -oout.txt -pd90.gvcpst01 -i2 -i3 -s(83000) -i0 -i4 -s() -i0 -s1 -s() -ri
