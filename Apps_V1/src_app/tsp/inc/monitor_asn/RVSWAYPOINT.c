/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "MONITOR"
 * 	found in "monitor.asn"
 */

#include "RVSWAYPOINT.h"

static int
memb_heading_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 359)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_speed_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= -1000 && value <= 4500)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_hdop_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 1000)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static int
memb_satellites_constraint_1(const asn_TYPE_descriptor_t *td, const void *sptr,
			asn_app_constraint_failed_f *ctfailcb, void *app_key) {
	long value;
	
	if(!sptr) {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: value not given (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
	
	value = *(const long *)sptr;
	
	if((value >= 0 && value <= 16)) {
		/* Constraint check succeeded */
		return 0;
	} else {
		ASN__CTFAIL(app_key, td, sptr,
			"%s: constraint failed (%s:%d)",
			td->name, __FILE__, __LINE__);
		return -1;
	}
}

static asn_oer_constraints_t asn_OER_memb_heading_constr_3 CC_NOTUSED = {
	{ 2, 1 }	/* (0..359) */,
	-1};
static asn_per_constraints_t asn_PER_memb_heading_constr_3 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 9,  9,  0,  359 }	/* (0..359) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_speed_constr_4 CC_NOTUSED = {
	{ 2, 0 }	/* (-1000..4500) */,
	-1};
static asn_per_constraints_t asn_PER_memb_speed_constr_4 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 13,  13, -1000,  4500 }	/* (-1000..4500) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_hdop_constr_5 CC_NOTUSED = {
	{ 2, 1 }	/* (0..1000) */,
	-1};
static asn_per_constraints_t asn_PER_memb_hdop_constr_5 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 10,  10,  0,  1000 }	/* (0..1000) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
static asn_oer_constraints_t asn_OER_memb_satellites_constr_6 CC_NOTUSED = {
	{ 1, 1 }	/* (0..16) */,
	-1};
static asn_per_constraints_t asn_PER_memb_satellites_constr_6 CC_NOTUSED = {
	{ APC_CONSTRAINED,	 5,  5,  0,  16 }	/* (0..16) */,
	{ APC_UNCONSTRAINED,	-1, -1,  0,  0 },
	0, 0	/* No PER value map */
};
asn_TYPE_member_t asn_MBR_RVSWAYPOINT_1[] = {
	{ ATF_NOFLAGS, 0, offsetof(struct RVSWAYPOINT, position),
		(ASN_TAG_CLASS_UNIVERSAL | (16 << 2)),
		0,
		&asn_DEF_RVSWGS84POINT,
		0,
		{ 0, 0, 0 },
		0, 0, /* No default value */
		"position"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RVSWAYPOINT, heading),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_heading_constr_3, &asn_PER_memb_heading_constr_3,  memb_heading_constraint_1 },
		0, 0, /* No default value */
		"heading"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RVSWAYPOINT, speed),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_speed_constr_4, &asn_PER_memb_speed_constr_4,  memb_speed_constraint_1 },
		0, 0, /* No default value */
		"speed"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RVSWAYPOINT, hdop),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_hdop_constr_5, &asn_PER_memb_hdop_constr_5,  memb_hdop_constraint_1 },
		0, 0, /* No default value */
		"hdop"
		},
	{ ATF_NOFLAGS, 0, offsetof(struct RVSWAYPOINT, satellites),
		(ASN_TAG_CLASS_UNIVERSAL | (2 << 2)),
		0,
		&asn_DEF_NativeInteger,
		0,
		{ &asn_OER_memb_satellites_constr_6, &asn_PER_memb_satellites_constr_6,  memb_satellites_constraint_1 },
		0, 0, /* No default value */
		"satellites"
		},
};
static const ber_tlv_tag_t asn_DEF_RVSWAYPOINT_tags_1[] = {
	(ASN_TAG_CLASS_UNIVERSAL | (16 << 2))
};
static const asn_TYPE_tag2member_t asn_MAP_RVSWAYPOINT_tag2el_1[] = {
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 1, 0, 3 }, /* heading */
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 2, -1, 2 }, /* speed */
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 3, -2, 1 }, /* hdop */
    { (ASN_TAG_CLASS_UNIVERSAL | (2 << 2)), 4, -3, 0 }, /* satellites */
    { (ASN_TAG_CLASS_UNIVERSAL | (16 << 2)), 0, 0, 0 } /* position */
};
asn_SEQUENCE_specifics_t asn_SPC_RVSWAYPOINT_specs_1 = {
	sizeof(struct RVSWAYPOINT),
	offsetof(struct RVSWAYPOINT, _asn_ctx),
	asn_MAP_RVSWAYPOINT_tag2el_1,
	5,	/* Count of tags in the map */
	0, 0, 0,	/* Optional elements (not needed) */
	-1,	/* First extension addition */
};
asn_TYPE_descriptor_t asn_DEF_RVSWAYPOINT = {
	"RVSWAYPOINT",
	"RVSWAYPOINT",
	&asn_OP_SEQUENCE,
	asn_DEF_RVSWAYPOINT_tags_1,
	sizeof(asn_DEF_RVSWAYPOINT_tags_1)
		/sizeof(asn_DEF_RVSWAYPOINT_tags_1[0]), /* 1 */
	asn_DEF_RVSWAYPOINT_tags_1,	/* Same as above */
	sizeof(asn_DEF_RVSWAYPOINT_tags_1)
		/sizeof(asn_DEF_RVSWAYPOINT_tags_1[0]), /* 1 */
	{ 0, 0, SEQUENCE_constraint },
	asn_MBR_RVSWAYPOINT_1,
	5,	/* Elements count */
	&asn_SPC_RVSWAYPOINT_specs_1	/* Additional specs */
};

