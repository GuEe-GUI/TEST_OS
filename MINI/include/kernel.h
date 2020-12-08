/* File: kernel.h */

#ifndef _KERNEL_H_
#define _KERNEL_H_

#define  KERN_EMERG     "<0>"   /* system is unusable                */
#define  KERN_ALERT     "<1>"   /* action must be taken immediately  */
#define  KERN_CRIT      "<2>"   /* critical conditions               */
#define  KERN_ERR       "<3>"   /* error conditions                  */
#define  KERN_WARNING   "<4>"   /* warning conditions                */
#define  KERN_NOTICE    "<5>"   /* normal but significant condition  */
#define  KERN_INFO      "<6>"   /* informational                     */
#define  KERN_DEBUG     "<7>"   /* debug-level messages              */

int printk(const char * fmt, ...);

#ifdef __DEBUG__
#define assert(CONDITION) \
if (!(CONDITION)) {\
setColor(0xff0000ff, 0x1e2229ff);\
printk(\
"\n!!!!!!!!!! OS Assert !!!!!!!!!!\n\
File: %s\n\
Line: %d\n\
Function: %s\n\
Expression: "\
#CONDITION\
"\n!!!!!!!!!! OS Assert !!!!!!!!!!\n",__FILE__, __LINE__, __func__);\
setColor(0x44883cff, 0x1e2229ff);\
}
#else
#define assert(x)
#endif

#endif
