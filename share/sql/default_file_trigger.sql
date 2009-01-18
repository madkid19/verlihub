INSERT IGNORE INTO file_trigger (command,send_as,def,descr,min_class,flags) VALUES
	('+motd','MOTD','%[CFG]/motd','Message of the day',0,4),
	('+motd_reg','MOTD','%[CFG]/motd_reg','Message of the day',1,4),
	('+motd_vip','MOTD','%[CFG]/motd_vip','Message of the day',2,4),
	('+motd_op','MOTD','%[CFG]/motd_op','Message of the day',3,4),
	('+motd_cheef','MOTD','%[CFG]/motd_cheef','Message of the day',4,4),
	('+motd_admin','MOTD','%[CFG]/motd_admin','Message of the day',5,4),
	('+motd_master','MOTD','%[CFG]/motd_master','Message of the day',10,4),
	('+help_usr','HELP','%[CFG]/help_usr','Help text',0,8),
	('+help_reg','HELP','%[CFG]/help_reg','Help text',1,8),
	('+help_vip','HELP','%[CFG]/help_vip','Help text',2,8),
	('+help_op','HELP','%[CFG]/help_op','Help text',3,8),
	('+help_cheef','HELP','%[CFG]/help_cheef','Help text',4,8),
	('+help_admin','HELP','%[CFG]/help_admin','Help text',5,8),
	('+help_master','HELP','%[CFG]/help_master','Help text',10,8),
	('+rules','RULES','%[CFG]/rules','Hub rules',0,2),
	('+faq','FAQ','%[CFG]/faq','Frequently asked quenstions',0,2),
	('+sysversion','SystemVersion','/proc/version','Operating System Version',5,4),
	('+admin_reminder','Reminder','
	Congratulations you successfully installed Verlihub%[VERSION]
	
	Please visit the Web sites for imformation about the Verlihub project. It can be found here http://www.verlihub-project.org
	
	Verlihub Admin Zone DC Hub: dchub://hub.verlihub-project.org:7777
	
	Verlihub discussion forum: http://forums.verlihub-project.org
	','Admin reminder',5,62),
	('+vh_donation','Donation','
	If you are interested in making a donation to the author or developers of the verlihub
	project please visit our website for more information. http://www.verlihub-project.org/donations.php

	To diable this message, type the command !modtrigger +vh_donation -c11 in mainchat.
	','Donation Reminder', 10, 62),
	('+updates','Updates','
	Many things have been fixed since RC1 and this is considered to be our most functional
	and stable release to date. While the functionality of the software is richer then ever,
	its resource usage is at an all time low. It is now an extremely scaleable server.
	All known exploits have now been permanently closed to help protect both you AND other
	hubs in the DC community.
	
	Get Rid of this message by typing the command !modtrigger +updates -c11 in mainchat.
	','Updates',5,62),
	('+news','News','
	This will be the final release candidate of the Verlihub-0.9.8d tree. Work has now begun
	on the next major version of verlihub with planned support for new protocol extensions as
	well as some significant new features.
	Thanks to everyone who contributes their free time and effort for all aspects of the verlihub project.
	
	Get Rid of this message by typing the command !modtrigger +news -c11 in mainchat.
	','News',5,62)
	
