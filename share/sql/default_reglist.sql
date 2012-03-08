INSERT IGNORE INTO reglist (nick,class,reg_date,reg_op,pwd_change,note_op) VALUES
	('pinger', -1,UNIX_TIMESTAMP(NOW()),'installation',0,'generic pinger nick'),
	('dchublist',-1,UNIX_TIMESTAMP(NOW()),'installation',0,'dchublist.com pinger'),
	('FearListPinger',-1,UNIX_TIMESTAMP(NOW()),'installation',0,'feardc hublist pinger'),
	('publichublist',-1,UNIX_TIMESTAMP(NOW()),'installation',0,'publichublist pinger'),
	('dchublist_ru',-1,UNIX_TIMESTAMP(NOW()),'installation',0,'dchublist.ru pinger')
