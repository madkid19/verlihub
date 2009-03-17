use newdb;

CREATE TABLE IF NOT EXISTS reglist (
nick varchar(60) NOT NULL PRIMARY KEY, 
class int(2) default 1, 
class_protect int(2) default 0, 
class_hidekick int(2) default 0, 
hide_kick bool default 0, 
reg_date int(11), 
reg_op varchar(60), 
pwd_change bool default 1, 
pwd_crypt bool default 0, 
login_pwd varchar(60), 
login_last int(11) default 0, 
logout_last int(11) default 0, 
login_cnt int(11) default 0, 
login_ip varchar(16), 
error_last int(11), 
error_cnt int(11) default 0, 
error_ip varchar(16), 
enabled bool default 1, 
email varchar(60), 
note_op text, 
note_usr text, 
INDEX login_index (login_last), 
INDEX logout_index (logout_last) 
);

insert into reglist (nick,class,reg_date,reg_op,pwd_change,pwd_crypt,login_pwd,login_last,login_cnt,login_ip,error_last,error_cnt,error_ip,enabled)
select nick,class,unix_timestamp(reg_date),reg_op,pwd_change,1,encrypt(login_pwd),unix_timestamp(login_last),login_count,login_ip,unix_timestamp(error_last),error_cnt,error_ip,enabled from olddb.reglist;
