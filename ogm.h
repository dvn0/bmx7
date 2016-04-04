/*
 * Copyright (c) 2010  Axel Neumann
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of version 2 of the GNU General Public
 * License as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA
 */






extern uint32_t ogms_pending;

#define ARG_OGM_IFACTOR "ogmIntervalFactor"
#define DEF_OGM_IFACTOR 110
#define MIN_OGM_IFACTOR 100
#define MAX_OGM_IFACTOR 1000


#define ARG_OGM_INTERVAL "ogmInterval"
#define DEF_OGM_INTERVAL 6000
#define MIN_OGM_INTERVAL 200
#define MAX_OGM_INTERVAL 60000 // 60000 = 1 minutes


#define MIN_OGM_AGGREG_HISTORY 2
#define MAX_OGM_AGGREG_HISTORY AGGREG_SQN_CACHE_RANGE
#define DEF_OGM_AGGREG_HISTORY 20
#define ARG_OGM_AGGREG_HISTORY "ogmAggregHistory"


#define MIN_SEND_REVISED_OGMS 0
#define DEF_SEND_REVISED_OGMS 20
#define MAX_SEND_REVISED_OGMS 99
#define ARG_SEND_REVISED_OGMS "sendRevisedOgms"
extern int32_t sendRevisedOgms;

#define OGM_JUMPS_PER_AGGREGATION 10


#define OGMS_DHASH_PER_AGGREG_PREF (SIGNED_FRAMES_SIZE_PREF - (\
                              sizeof(struct tlv_hdr) + \
                              sizeof (struct hdr_ogm_adv))) \
                              / sizeof(struct msg_ogm_adv)



#define OGM_IID_RSVD_JUMP  (OGM_IIDOFFST_MASK) // 63 //255 // resulting from ((2^transmitterIIDoffset_bit_range)-1)

struct msg_ogm_aggreg_sqn_adv {
	AGGREG_SQN_T max;
	uint16_t size;
} __attribute__((packed));

struct msg_ogm_aggreg_req {
	AGGREG_SQN_T sqn;
} __attribute__((packed));

struct hdr_ogm_aggreg_req {
	GLOBAL_ID_T dest_nodeId;
	struct msg_ogm_aggreg_req msg[];
} __attribute__((packed));
/*
 *            short long
 * sqnHashLink  112  112
 * shortDhash    15   16
 * sqn           13   13
 * flags          1    8
 * metric        11   11
 * ---------------------
 *              152  160
 * */

struct msg_ogm_adv {
	ChainLink_T chainOgm;
	IID_T transmitterIID4x;
	OGM_SQN_T ogmSqn_remove;
	union {

		struct {
			unsigned int TODO_consider_iid_here : 14;
			unsigned int trustedFlag : 1;
			unsigned int hopCount : 6;
			unsigned int metric_exp : OGM_EXPONENT_BIT_SIZE; // 5
			unsigned int metric_mantissa : OGM_MANTISSA_BIT_SIZE; // 6
		} __attribute__((packed)) f;
		uint32_t u32;
	} u;

} __attribute__((packed));

struct hdr_ogm_adv {
	AGGREG_SQN_T aggregation_sqn;
	struct msg_ogm_adv msg[];
} __attribute__((packed));

struct avl_tree **get_my_ogm_aggreg_origs(AGGREG_SQN_T aggSqn);

void remove_ogm(struct orig_node *on);
void process_ogm_metric(void *voidRef);

int32_t init_ogm(void);
