#!/bin/sh

if [ ! -d "/run/mysqld" ]; then
	mkdir -p /run/mysqld
	chown -R mysql:mysql /run/mysqld
fi

if [ ! -f /var/lib/mysql/mariadb_flag ]
then

  chown -R mysql:mysql /var/lib/mysql

  mysql_install_db --basedir=/usr --datadir=/var/lib/mysql --user=mysql --rpm > /dev/null

  echo "CREATE DATABASE IF NOT EXISTS ${MYSQL_DATABASE};" > initial.sql
  echo "CREATE USER '${MYSQL_USER}'@'%' IDENTIFIED BY '${MYSQL_PASSWORD}';" >> initial.sql
  echo "GRANT ALL PRIVILEGES ON *.* TO '${MYSQL_USER}'@'%';" >> initial.sql
  echo "FLUSH PRIVILEGES;" >> initial.sql
  echo "ALTER USER 'root'@'localhost' IDENTIFIED BY '${MYSQL_ROOT_PASSWORD}';" >> initial.sql
  ##
  echo $(cat initial.sql)
  echo $(ls -al)

  touch /var/lib/mysql/mariadb_flag

  mysqld_safe --init-file=$PWD/initial.sql
else
  mysqld_safe
fi
