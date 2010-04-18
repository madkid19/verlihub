/***************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2009 by Verlihub Project                           *
*   devs at verlihub-project dot org                                      *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#ifndef CDCCONF_H
#define CDCCONF_H
#include "cconfigbase.h"
#include "cmessagedc.h"
#include "cdctag.h"

using namespace nConfig;
using std::string;
namespace nDirectConnect {

namespace nProtocol {
	class cDCProto;
};

using nProtocol::cDCProto;
using nProtocol::nEnums::eDC_UNKNOWN;

class cServerDC;
class cDCBanList;
class cDCConsole;
class cConnDC;


namespace nTables {

	class cDCBanList;

/**dc configuration
  *@author Daniel Muller
  * This class contains almost all verlihub's configuration parameters
  */

class cDCConf : public cConfigBase //<sBasicItemCreator>
{
public:
	cDCConf( cServerDC &);
	~cDCConf();
	virtual int Load();
	virtual int Save();
	void AddVars();

	friend class ::nDirectConnect::cServerDC;
	friend class ::nDirectConnect::nProtocol::cDCProto;
	friend class ::nDirectConnect::nTables::cDCBanList;
	friend class ::nDirectConnect::cDCConsole;
	friend class ::nDirectConnect::cConnDC;
public:
	int max_users_total;
	int max_users[7];
	int max_extra_regs;
	int max_extra_vips;
	int max_extra_ops;
	int max_extra_cheefs;
	int max_extra_admins;
	double max_upload_kbps;
	unsigned long min_share;
	unsigned long min_share_reg;
	unsigned long min_share_vip;
	unsigned long min_share_ops;
	double min_share_factor_passive;
	unsigned long max_share;
	unsigned long max_share_reg;
	unsigned long max_share_vip;
	unsigned long max_share_ops;
	unsigned long min_share_use_hub;
	unsigned long min_share_use_hub_reg;
	unsigned long min_share_use_hub_vip;
	int min_class_use_hub;
	int min_class_use_hub_passive;
	unsigned int max_passive_sr;
	unsigned tban_kick;
	unsigned tban_max;
	unsigned int max_length[eDC_UNKNOWN+1];
	unsigned max_nick;
	unsigned min_nick;
	string nick_chars;
	unsigned max_chat_msg;
	int max_chat_lines;
	int max_flood_counter_pm;
	int nicklist_on_login;
	bool delayed_login;
	int delayed_search;
	int delayed_myinfo; // implies also delayed quit.
	bool drop_invalid_key;
	bool delayed_chat;
	int delayed_ping;
	double min_frequency;
	string nick_prefix;
	string nick_prefix_autoreg;
	bool nick_prefix_cc;
	int host_header;
	string hub_security;
	string hub_category;
	string hub_security_desc;
	string opchat_name;
	string opchat_desc;
	string cmd_start_op;
	string cmd_start_user;
	bool dest_report_chat;
	bool dest_regme_chat;
	bool dest_drop_chat;
	bool report_dns_lookup;
	string extra_listen_ports;
	string hub_version_special;
	string hub_name;
	string hub_version;
	string hub_topic;	
	string hub_desc;
	string hub_host;
	string hub_owner;
	string hublist_host;
	int hublist_port;
	bool hublist_send_minshare;
	/** checking prefferences */
	unsigned classdif_reg;
	unsigned classdif_search;
	unsigned classdif_download;
	unsigned classdif_pm;
	unsigned classdif_kick;
	int min_class_register;
	int min_class_bc;
	int min_class_bc_guests;
	int min_class_bc_regs;
	int min_class_bc_vips;
	int min_class_redir;
	int max_class_int_login;
	int hide_all_kicks;
	int optimize_userlist;
	int search_number;
	int int_search;
	int int_search_pas;
	int int_search_reg_pass;
	int int_search_reg;
	int int_search_vip;
	int int_search_op;
	int int_login;
	int int_flood_pm_period;
	int int_flood_pm_limit;
	int int_chat_ms;
	int int_nicklist;
	int int_myinfo;
	int disable_me_cmd; // TODO: Convert to bool
	int disable_regme_cmd;
	int disable_usr_cmds; 
	int disable_report_cmd;
	int plugin_mod_class;
	int topic_mod_class;
	int mainchat_class;
	int ul_portion;
	int min_search_chars;
	string ip_zone4_min;
	string ip_zone4_max;
	string ip_zone5_min;
	string ip_zone5_max;
	string ip_zone6_min;
	string ip_zone6_max;

	int tag_max_hubs;
	double tag_max_hs_ratio;
	bool tag_allow_uknown;
	bool tag_allow_none;
	bool tag_allow_sock5;
	bool tag_allow_passive;
	int tag_min_class_ignore;
	int tag_sum_hubs;
	string cc_zone[3];
	int show_tags; // 0 - no at all; 1 - only to ops, 2- only to everyone ;)
	int show_desc_len; // cut first n bytes of description
	int autoreg_class;
	int show_email;
	int show_speed;
	bool send_user_ip;
	bool send_user_info;
	bool use_reglist_cache;
	bool use_penlist_cache;
	bool chat_default_on;
	bool allways_ask_password;
	int default_password_encryption;
	int password_min_len;
	int pwd_tmpban;
	int wrongpassword_report;
	int botinfo_report;
	double timeout_length[6];

	string msg_banned;
	string msg_banned_client;
	string msg_hub_full;
	string msg_nick_prefix;
	string msg_chat_onoff;
	string msg_change_pwd;
	string msg_downgrade;
	string msg_upgrade;
	string msg_replace_ban;
	string msg_welcome[11];
	string search_share_min;
	string ctm_share_min;
	string autoreg_min_share;
	string login_share_min;
	string login_share_max;

	bool save_lang;
	bool desc_insert_mode;
public: // Public attributes
	cServerDC & mS;
};

class cDCLang : public cConfigBase //<sBasicItemCreator>
{
public:
	cDCLang( cServerDC &);
	~cDCLang();
	virtual int Load();
	virtual int Save();
	void AddVars();
	cServerDC &mS;

	string ban_reason;
	string ban_remaining;
	string ban_expired;
	string ban_for;
	string ban_permanently;
	string ban_types[15];
	string ban_type;
	string ban_removed;
	string ban_by;

	string chat_msg_long;
	string chat_msg_lines;
	string pwd_cannot;
	string pwd_can;
	string pwd_min;
	string pwd_success;
	string pwd_set_error;
	string pwd_setup;
	string ip;
	string nick;
	string user;
	string host;
	string ip_range;
	string because;
	string op;
	string type;
	string not_in_userlist;
	string error;
	string success;
	string wrong_dc_tag;
	string tag_max_hubs;
	string tag_max_slots;
	string tag_min_slots;
	string tag_max_hs_ratio;
	string tag_min_limit;
	string tag_min_ls_ratio;
	string tag_no_sock;
	string tag_no_passive;
	
	
	
	string msg_downgrade;
	string msg_upgrade;
	string msg_topic_set;
	string msg_topic_reset;
	
	string autoreg_nick_prefix;
	string autoreg_already_reg;
	string autoreg_success;
	string autoreg_error;

	string timeout_any;
	string operation_timeout;
	string timeout_text[6];
};
};
};
#endif
