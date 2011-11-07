/**************************************************************************
*   Original Author: Daniel Muller (dan at verliba dot cz) 2003-05        *
*                                                                         *
*   Copyright (C) 2006-2011 by Verlihub Project                           *
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

using std::string;
namespace  nVerliHub {

	namespace nProtocol {
		class cDCProto;
	};
	using nProtocol::cDCProto;
	class cDCBanList;
	class cDCConsole;
	namespace nSocket {
		class cConnDC;
		class cServerDC;
	};


namespace nTables {

	class cDCBanList;

/**dc configuration
  *@author Daniel Muller
  * This class contains almost all verlihub's configuration parameters
  */

class cDCConf : public nConfig::cConfigBase //<sBasicItemCreator>
{
public:
	cDCConf(nSocket::cServerDC &);
	~cDCConf();
	virtual int Load();
	virtual int Save();
	void AddVars();

	friend class nVerliHub::nSocket::cServerDC;
	friend class nProtocol::cDCProto;
	friend class nTables::cDCBanList;
	friend class cDCConsole;
	friend class nVerliHub::nSocket::cConnDC;
public:
	unsigned int max_users_total;
	int max_users_passive;
	unsigned int max_users_from_ip;
	unsigned int max_users[7];
	unsigned int max_extra_regs;
	unsigned int max_extra_vips;
	unsigned int max_extra_ops;
	unsigned int max_extra_cheefs;
	unsigned int max_extra_admins;
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
	unsigned int max_length[nEnums::eDC_UNKNOWN+1];
	unsigned max_nick;
	unsigned min_nick;
	string nick_chars;
	unsigned max_chat_msg;
	int max_chat_lines;
	int max_flood_counter_pm;
	int max_flood_counter_mcto;
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
	bool extended_welcome_message;
	unsigned int host_header;
	string hub_security;
	string hub_category;
	string hub_icon_url;
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
	unsigned classdif_mcto;
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
	int int_flood_mcto_period;
	int int_flood_mcto_limit;
	int int_chat_ms;
	int int_nicklist;
	int int_myinfo;
	int disable_me_cmd; // TODO: Convert to bool
	int disable_regme_cmd;
	int disable_usr_cmds;
	int disable_report_cmd;
	bool disable_zlib;
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
	int tag_min_hubs;
	int tag_min_hubs_usr;
	int tag_min_hubs_reg;
	int tag_min_hubs_op;
	double tag_max_hs_ratio;
	bool tag_allow_unknown;
	bool tag_allow_none;
	bool tag_allow_sock5;
	bool tag_allow_passive;
	int tag_min_class_ignore;
	int tag_sum_hubs;
	double tag_min_version;
	double tag_max_version;
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
	string wrongpass_message;
	int wrongpassword_report;
	bool wrongauthip_report;
	int botinfo_report;
	double timeout_length[6];

	string ban_extra_message;
	string msg_replace_ban;
	string msg_welcome[11];
	bool desc_insert_mode;
public: // Public attributes
	nSocket::cServerDC & mS;
};
	}; // nmaespace nTables
}; // namespace nVerliHub
#endif
