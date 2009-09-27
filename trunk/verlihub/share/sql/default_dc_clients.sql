# http://www.fin-overclocking.net/Profiles.xml
# http://btpro.altervista.org/Tags.htm
#
#              <oDC V:5.31,M:A,H:0/0/2,S:9>
#              <DCGUI V:0.3.4cvs20041024,M:A,H:2,S:2>
#              <StrgDC++ V:1.00 RC3,M:A,H:0/3/2,S:2>
#              <DC:PRO V:0.2.3.0,M:A,H:1/0/0,S:4>
#              <DC V:2.20,M:A,H:5/0/0,S:5,O:2>
#              <rmDC++ V:0.403B[7.01],M:A,H:0/0/1,S:15>
#              <++ V:0.403,M:A,H:0/0/5,S:1>
#     <iDC1.24><++ V:0.404,M:A,H:0/0/8,S:3>
# zDC++[0.401b]<++ V:0.403,M:A,H:0/0/1,S:15>
#  <DCDM 0.041><++ V:0.401,M:A,H:1/0/4,S:3>
#  

-- INSERT IGNORE INTO dc_clients (name, prefix_regex, prefix_version_rank, intag_id, per_slot_limit) VALUES
-- 	('DC++',NULL,-1,'++',0),
-- 	('Valknut',NULL,-1,'DCGUI',1),
-- 	('oDC',NULL,-1,'oDC',0),
-- 	('oDC-2','o([0-9.]+)',1,'++',0),
-- 	('Strong DC++',NULL,-1,'StrgDC\\+\\+',0),
-- 	('ApexDC',NULL,-1,'ApexDC',0),
-- 	('DC Pro',NULL,-1,'DC:PRO',0),
-- 	('NMDC',NULL,-1,'DC',0),
-- 	('iDC++','<iDC([0-9.]+)>',1,'++',0),
-- 	('zDC++','zDC\\+\\+\\[([0-9.]+)\\]',1,'++',0),
-- 	('Z++',NULL, -1, 'Z++',0)
INSERT IGNORE INTO dc_clients (name, tag_id, min_version, max_version, ban, enable) VALUES
	('DC++','++',-1,-1,0,1),
	('Valknut','DCGUI',-1,-1,0,1),
	('oDC','oDC',-1,-1,0,1),
	('oDC-2','o',-1,-1,0,1),
	('Strong DC++','StrgDC++',-1,-1,0,1),
	('ApexDC','ApexDC',-1,-1,0,1),
	('DC Pro','DC:PRO',-1,-1,0,1),
	('NMDC','DC',-1,-1,0,1),
	('iDC++','iDC',-1,-1,0,1),
	('zDC++','zDC++',-1,-1,0,1),
	('Z++','Z++',-1,-1,0,1)
