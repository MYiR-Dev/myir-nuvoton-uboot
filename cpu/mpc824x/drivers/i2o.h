#ifndef I2O_H
#define I2O_H
/*********************************************************
 *
 * copyright @ Motorola, 1999
 *********************************************************/

#define I2O_REG_OFFSET 0x0004

#define PCI_CFG_CLA    0x0B
#define PCI_CFG_SCL    0x0A
#define PCI_CFG_PIC    0x09

#define I2O_IMR0 0x0050
#define I2O_IMR1 0x0054
#define I2O_OMR0 0x0058
#define I2O_OMR1 0x005C

#define I2O_ODBR 0x0060
#define I2O_IDBR 0x0068

#define I2O_OMISR  0x0030
#define I2O_OMIMR  0x0034
#define I2O_IMISR  0x0100
#define I2O_IMIMR  0x0104

/* accessable to PCI master but local processor */
#define I2O_IFQPR  0x0040
#define I2O_OFQPR  0x0044

/* accessable to local processor */
#define I2O_IFHPR  0x0120
#define I2O_IFTPR  0x0128
#define I2O_IPHPR  0x0130
#define I2O_IPTPR  0x0138
#define I2O_OFHPR  0x0140
#define I2O_OFTPR  0x0148
#define I2O_OPHPR  0x0150
#define I2O_OPTPR  0x0158
#define I2O_MUCR   0x0164
#define I2O_QBAR   0x0170

#define I2O_NUM_MSG 2

typedef enum _i2o_status
{
	I2OSUCCESS = 0,
	I2OINVALID,
	I2OMSGINVALID,
	I2ODBINVALID,
	I2OQUEINVALID,
	I2OQUEEMPTY,
	I2OQUEFULL,
	I2ONOEVENT,
} I2OSTATUS;

typedef enum _queue_size
{
    QSIZE_4K = 0x02,
    QSIZE_8K = 0x04,
    QSIZE_16K = 0x08,
    QSIZE_32K = 0x10,
    QSIZe_64K = 0x20,
} QUEUE_SIZE;

typedef enum _location
{
    LOCAL = 0,     /* used by local processor to access its own on board device,
		      local processor's eumbbar is required */
    REMOTE,        /* used by PCI master to access the devices on its PCI device,
		      device's pcsrbar is required */
} LOCATION;

/* door bell */
typedef enum _i2o_in_db
{
  IN_DB = 1,
  MC,         /* machine check */
} I2O_IN_DB;

/* I2O PCI configuration identification */
typedef struct _i2o_iop
{
	unsigned int base_class : 8;
	unsigned int sub_class  : 8;
	unsigned int prg_code   : 8;
} I2OIOP;

/* I2O Outbound Message Interrupt Status Register */
typedef struct _i2o_om_stat
{
	unsigned int rsvd0 : 26;
	unsigned int opqi  : 1;
	unsigned int rsvd1 : 1;
	unsigned int odi   : 1;
	unsigned int rsvd2 : 1;
	unsigned int om1i  : 1;
	unsigned int om0i  : 1;
} I2OOMSTAT;

/* I2O inbound Message Interrupt Status Register */
typedef struct _i2o_im_stat
{
	unsigned int rsvd0 : 23;
	unsigned int ofoi  : 1;
	unsigned int ipoi  : 1;
	unsigned int rsvd1 : 1;
	unsigned int ipqi  : 1;
	unsigned int mci   : 1;
	unsigned int idi   : 1;
	unsigned int rsvd2 : 1;
	unsigned int im1i  : 1;
	unsigned int im0i  : 1;
} I2OIMSTAT;

/**
 Enable the interrupt associated with in/out bound msg

 Inbound message interrupt generated by PCI master and serviced by local processor
 local processor needs to enable its inbound interrupts it wants to handle (LOCAL)

 Outbound message interrupt generated by local processor and serviced by PCI master
 PCI master needs to enable the devices' outbound interrupts it wants to handle (REMOTE)
 **/
extern I2OSTATUS I2OMsgEnable( LOCATION,            /*  REMOTE/LOCAL   */
                               unsigned int base,   /* pcsrbar/eumbbar */
                               unsigned char n );   /* b'1' - msg 0
						                             * b'10'- msg 1
						                             * b'11'- both
						                             */

/**
 Disable the interrupt associated with in/out bound msg

 local processor needs to disable its inbound interrupts it is not interested (LOCAL)

 PCI master needs to disable outbound interrupts of devices it is not interested (REMOTE)
 **/
extern I2OSTATUS I2OMsgDisable( LOCATION,          /*  REMOTE/LOCAL   */
                                unsigned int base, /* pcsrbar/eumbbar */
                                unsigned char n ); /* b'1' - msg 0
			   			                            * b'10'- msg 1
						                            * b'11'- both
						                            */

/**
 Read the msg register either from local inbound msg 0/1,
 or an outbound msg 0/1 of devices.

 If it is not local, pcsrbar must be passed to the function.
 Otherwise eumbbar is passed.

 If it is remote, outbound msg of the device is read.
 Otherwise local inbound msg is read.
 **/
extern I2OSTATUS I2OMsgGet ( LOCATION,                 /* REMOTE/LOCAL */
                             unsigned int base,        /*pcsrbar/eumbbar */
                             unsigned int n,           /* 0 or 1 */
                             unsigned int *msg );

/**
 Write to nth Msg register either on local outbound msg 0/1,
 or aninbound msg 0/1 of devices

 If it is not local, pcsrbar must be passed to the function.
 Otherwise eumbbar is passed.

 If it is remote, inbound msg on the device is written.
 Otherwise local outbound msg is written.
 **/
extern I2OSTATUS I2OMsgPost( LOCATION,                 /* REMOTE/LOCAL */
                                unsigned int base,        /*pcsrbar/eumbbar */
                                unsigned int n,           /* 0 or 1 */
                                unsigned int msg );

/**
 Enable the In/Out DoorBell Interrupt

 InDoorBell interrupt is generated by PCI master and serviced by local processor
 local processor needs to enable its inbound doorbell interrupts it wants to handle

 OutDoorbell interrupt is generated by local processor and serviced by PCI master
 PCI master needs to enable outbound doorbell interrupts of the devices it wants to handle
 **/
extern I2OSTATUS I2ODBEnable( LOCATION,            /*  REMOTE/LOCAL   */
                              unsigned int base,   /* pcsrbar/eumbbar */
                              unsigned int in_db );/* when LOCAL, I2O_IN_DB, MC, I2O_IN_DB|MC */

/**
 Disable the In/Out DoorBell Interrupt

 local processor needs to disable its inbound doorbell interrupts it is not interested

 PCI master needs to disable outbound doorbell interrupts of devices it is not interested

 **/
extern I2OSTATUS I2ODBDisable( LOCATION,              /*  REMOTE/LOCAL   */
                               unsigned int base,     /* pcsrbar/eumbbar */
                               unsigned int in_db );  /* when LOCAL, I2O_IN_DB, MC, I2O_IN_DB|MC */

/**
 Read a local indoorbell register, or an outdoorbell of devices.
 Reading a doorbell register, the register will be cleared.

 If it is not local, pcsrbar must be passed to the function.
 Otherwise eumbbar is passed.

 If it is remote, outdoorbell register on the device is read.
 Otherwise local in doorbell is read
 **/
extern unsigned int I2ODBGet( LOCATION,             /*  REMOTE/LOCAL   */
                              unsigned int base);   /* pcsrbar/eumbbar */

/**
 Write to a local outdoorbell register, or an indoorbell register of devices.

 If it is not local, pcsrbar must be passed to the function.
 Otherwise eumbbar is passed.

 If it is remote, in doorbell register on the device is written.
 Otherwise local out doorbell is written
 **/
extern void I2ODBPost( LOCATION,                 /*  REMOTE/LOCAL   */
                       unsigned int base,        /* pcsrbar/eumbbar */
                       unsigned int msg );       /*   in   / out    */

/**
 Read the outbound msg unit interrupt status of devices. Reading an interrupt status register,
 the register will be cleared.

 The outbound interrupt status is AND with the outbound
 interrupt mask. The result is returned.

 PCI master must pass the pcsrbar to the function.
 **/
extern I2OSTATUS I2OOutMsgStatGet( unsigned int pcsrbar, I2OOMSTAT * );

/**
 Read the inbound msg unit interrupt status. Reading an interrupt status register,
 the register will be cleared.

 The inbound interrupt status is AND with the inbound
 interrupt mask. The result is returned.

 Local process must pass its eumbbar to the function.
**/
extern I2OSTATUS I2OInMsgStatGet( unsigned int eumbbar, I2OIMSTAT * );

/**
 Configure the I2O FIFO, including QBAR, IFHPR/IFTPR,IPHPR/IPTPR,OFHPR/OFTPR, OPHPR/OPTPR,
 MUCR.
 **/
extern I2OSTATUS I2OFIFOInit( unsigned int eumbbar,
				              QUEUE_SIZE,
				              unsigned int qba);/* queue base address that must be aligned at 1M */
/**
 Enable the circular queue
 **/
extern I2OSTATUS I2OFIFOEnable( unsigned int eumbbar );

/**
 Disable the circular queue
 **/
extern void I2OFIFODisable( unsigned int eumbbar );

/**
 Enable the circular queue interrupt
 PCI master enables outbound FIFO interrupt of device
 Device enables its inbound FIFO interrupt
 **/
extern void I2OFIFOIntEnable( LOCATION, unsigned int base  );

/**
 Disable the circular queue interrupt
 PCI master disables outbound FIFO interrupt of device
 Device disables its inbound FIFO interrupt
 **/
extern void I2OFIFOIntDisable( LOCATION, unsigned int base );

/**
 Enable the circular queue overflow interrupt
 **/
extern void I2OFIFOOverflowIntEnable( unsigned int eumbbar );

/**
 Disable the circular queue overflow interrupt
 **/
extern void I2OFIFOOverflowIntDisable( unsigned int eumbbar );

/**
 Allocate a free msg frame from free FIFO.

 PCI Master allocates a free msg frame through inbound queue port of device(IFQPR)
 while local processor allocates a free msg frame from outbound free queue(OFTPR)

 Unless both free queues are initialized, allocating a free MF will return 0xffffffff
 **/
extern I2OSTATUS I2OFIFOAlloc( LOCATION,
			 	               unsigned int base,
				               void         **pMsg);
/**
 Free a used msg frame back to free queue
 PCI Master frees a MFA through outbound queue port of device(OFQPR)
 while local processor frees a MFA into its inbound free queue(IFHPR)

 Used msg frame does not need to be recycled in the order they
 read

 This function has to be called by PCI master to initialize Inbound free queue
 and by device to initialize Outbound free queue before I2OFIFOAlloc can be used.
 **/
extern I2OSTATUS I2OFIFOFree( LOCATION,
			                  unsigned int base,
			                  void        *pMsg );

/**
 Post a msg into FIFO
 PCI Master posts a msg through inbound queue port of device(IFQPR)
 while local processor post a msg into its outbound post queue(OPHPR)

 The total number of msg must be less than the max size of the queue
 Otherwise queue overflow interrupt will assert.
 **/
extern I2OSTATUS I2OFIFOPost( LOCATION,
		                      unsigned int base,
		                      void         *pMsg );

/**
 Read a msg from FIFO
 PCI Master reads a msg through outbound queue port of device(OFQPR)
 while local processor reads a msg from its inbound post queue(IPTPR)
 **/
extern I2OSTATUS I2OFIFOGet( LOCATION,
	 		                  unsigned int base,
							  void     **pMsg );

/**
 Get the I2O PCI configuration identification register
 **/
extern I2OSTATUS I2OPCIConfigGet( LOCATION,
			                   unsigned int base,
							   I2OIOP *);

#endif
