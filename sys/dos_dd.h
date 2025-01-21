/**
Þ * Dos Driver Header
 */

#define	DOS_CMDS	25	/* # of DOS commands */
#define STK_SIZE	512	/* DOS Device Drive Stack */
#define	DEVICES		1	/* # of block devices */
#define	OP_COMPLETE	0x0000	/* Op complete return code */

#define CHAR_DD		0x8000  /* Character device driver */
#define IOCTL_SUP	0x4000  /* IOCTL supported */
#define NON_IBM		0x2000  /* Non-IBM format (Block) */
#define REMOVABLE	0x0800  /* Removable media (block */
#define GET_SET		0x0040  /* Get/Set Logical Device */
#define CLOCK_DD	0x0008  /* Current Clock Device */
#define NUL_DD          0x0004  /* Current NUL device */
#define STDOUT_DD       0x0002  /* Current stdout device */
#define STDIN_DD	0x0001  /* Current stdin device */
#define GEN_IOCTL	0x0001  /* Generic IOCTL if Block Device */

struct	DEVICE_HEADER_struct
{
    struct DEVICE_HEADER_struct	far *next_hdr;
    unsigned int  attribute;	/* Device Driver Attributes */
    unsigned int  dev_strat;	/* ptr to strategy code */
    unsigned int  dev_int;      /* ptr to interrupt code */
    unsigned char name_unit[8]; /* name/unit field */
};

#define ERROR_BIT	0x8000	/* Error Bit Mask */
#define ERROR_NUM	0x00FF	/* Error Number Mask */
#define DONE_BIT	0x0100	/* Device Operation Done */
#define BUSY_BIT	0x0200	/* Device Busy (not done) */

#define WRITE_PROTECT	0x00	/* Write Protect Violation */
#define UNKNOWN_UNIT	0x01	/* Unit Not Known by Driver */
#define NOT_READY	0x02	/* Device is not ready */
#define UNKNOWN_CMD	0x03	/* Unknown Device Command */
#define CRC_ERROR	0x04	/* Device CRC Error */
#define BAD_REQ_LEN	0x05	/* Bad Drive Req Struct Len */
#define SEEK_ERROR	0x06	/* Device Seek Error */
#define UNKNOWN_MEDIA	0x07	/* Unknown Media in Drive */
#define NOT_FOUND	0x08	/* Sector not Found */
#define OUT_OF_PAPER	0x09	/* Printer out of Paper */
#define WRITE_FAULT	0x0A	/* Device Write Fault */
#define READ_FAULT	0x0B	/* Device Read Fault */
#define GENERAL_FAIL	0x0C	/* General Device Failure */

/* Device Driver Command codes */

#define INIT		0	/* Initialize Device */
#define	MEDIA_CHECK	1	/* Check for Correct Media */
#define BUILD_BPB	2	/* Build a BIOS Param Block (BPB) */
#define IOCTL_INPUT	3	/* IOCTL Input Requested */
#define INPUT		4	/* Device Read Operation */
#define INPUT_NO_WAIT	5	/* Input No Wait (CHAR ONLY) */
#define INPUT_STATUS	6	/* Input Status (CHAR ONLY) */
#define INPUT_FLUSH	7	/* Input Flush (CHAR ONLY) */
#define OUTPUT		8	/* Device Write Operation */
#define OUTPUT_VERIFY	9	/* Device Write with Verify */
#define OUTPUT_STATUS	10	/* Output Status (CHAR ONLY) */
#define OUTPUT_FLUSH	11	/* Output Flush (CHAR ONLY) */
#define IOCTL_OUTPUT	12	/* IOCTL Output Requested */
#define DEV_OPEN	13	/* Device Open Command */
#define DEV_CLOSE	14	/* Device Close Command */
#define REMOVE_MEDIA	15	/* Remove Media command (eject?) */
#define RESERVED_1	16	/* Reserved Command 1 */
#define RESERVED_2	17	/* Reserved Command 2 */
#define RESERVED_3	18	/* Reserved Command 3 */
#define IOCTL		19	/* IOCTL */
#define RESERVED_4	20	/* Reserved Command 4 */
#define RESERVED_5	21	/* Reserved Command 5 */
#define RESERVED_6	22	/* Reserved Command 6 */
#define	GET_L_D_MAP	23	/* Get Logical Drive Map */
#define SET_L_D_MAP	24	/* Set Logical Drive Map */

struct	INIT_struct
{
	unsigned char num_of_units;
	unsigned char far *end_ptr;	/* end addr of driver */
	unsigned char far *BPB_ptr;	/* pointer to init args */
					/* Set to BPB array on exit */
	unsigned char drive_num;	/* driver # */
	unsigned int  config_err;	/* config.sys error flag */
};

struct	MEDIA_CHECK_struct
{
	unsigned char media_byte;	/* Media Descriptor from MS-DOS */
	unsigned char return_info;	/* Return Information */
	unsigned char far *return_ptr;	/* Pointer to prev VolID */
};

struct	BUILD_BPB_struct
{
	unsigned char	media_byte;	/* Media Descriptor from MS-DOS */
	unsigned char	far *buffer_ptr; /* Pointer to buffer */
	struct BPB_struct far *BPB_table; /* Pointer to BPB Table */
};

struct I_O_struct
{
	unsigned char media_byte;	/* Media Desc from MS-DOS */
	unsigned char far *buffer_ptr;	/* Pointer To Buffer */
	unsigned int  count;		/* Byte/Sector Count */
	unsigned int  start_sector;	/* Starting sector number */
	unsigned char far *vol_id_ptr;	/* ptr to Volume ID */
	unsigned long start_sector_32;	/* 32-bit starting sector */
};

struct INPUT_NO_WAIT_struct
{
	unsigned char byte_read;	/* byte read from device */
};

struct IOCTL_struct
{
	unsigned char major_func;	/* Function (Major) */
	unsigned char minor_func;	/* Function (Minor) */
	unsigned int  SI_reg;		/* Contents of SI Register */
	unsigned int  DI_reg;		/* Contents of DI Register */
					/* ptr to Request Packet */
	unsigned char far *ioctl_req_ptr;
};

struct L_D_MAP_struct
{
	unsigned char unit_code;	/* Input - Unit Code */
					/* output - last device used */
	unsigned char cmd_code;		/* Command Code */
	unsigned int  status;		/* Status Word */
	unsigned long reserved;  	/* DOS Reserved */
};

struct REQ_struct
{
	unsigned char length;		/* Length in bytes of Request */
	unsigned char unit;		/* Minor Device Unit Number */
	unsigned char command;		/* Device Command Code */
	unsigned int  status;		/* Device Status Word */
	unsigned char reserved[8];	/* Reserved for MS-DOS */
	union
	{
		struct	INIT_struct		init_req;
		struct	MEDIA_CHECK_struct	media_check_req;
		struct	BUILD_BPB_struct	build_bpb_req;
		struct	I_O_struct		i_o_req;
		struct	INPUT_NO_WAIT_struct	ioctl_req;
		struct	L_D_MAP_struct		l_d_map_req;
	} req_type;
};

struct	BPB_struct
{
	unsigned int  bps;		/* Bytes per Sector */
	unsigned char spau;		/* Sectors/allocation unit */
	unsigned int  rs;		/* Reserved Sectors */
	unsigned char num_FATs;		/* # of FATs */
	unsigned int  root_entries;	/* # of Root Dir Entries */
	unsigned int  num_sectors;      /* # of sectors */
	unsigned char media_descriptor; /* Media Descriptor */
	unsigned int  spfat;		/* # of sectors per FAT */
	unsigned int  spt;		/* # of sectors per track */
	unsigned int  heads;		/* # of heads */
	unsigned long hidden;		/* # of hidden sectors */
	unsigned long num_of_sectors_32; /* 32-bit # of sectors */
};
