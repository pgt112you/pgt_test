#coding:utf-8
import sqlite3

conn = sqlite3.connect('/data0/home/guangtong/sltest.db')
c = conn.cursor()

sqlstr = """
    select * from sqlite_master;
"""
cursor =c.execute(sqlstr)
for row in cursor:
    print row
conn.commit()
conn.close()
