#include <linux/lsm_hooks.h>
#include <linux/kern_levels.h>
#include <linux/binfmts.h>
#include <uapi/linux/lsm.h>
#include <linux/sched.h>
#define MAX_PATHLEN 128
typedef struct mysec_secinfo {
    char pathname[MAX_PATHLEN];
    int pathlen;
} mysec_secinfo;
static int mysec_bprm_check_security(struct linux_binprm *bprm)
{
    printk(KERN_EMERG "statesec started %s\n", bprm->interp);
    return 0;
}
static int mysec_task_alloc_security(struct task_struct * p, long unsigned int x){
    mysec_secinfo *sec = NULL;
    mysec_secinfo *parsec = NULL;
    int err = 0;
    printk(KERN_EMERG"making a parent \n");
    sec = (mysec_secinfo *)kmalloc(sizeof(mysec_secinfo), GFP_KERNEL);
    parsec = current->security;
	if (!parsec) {
		sec->pathname[0] = '\0';
		sec->pathlen = 0;
	}
	else {
		//printk(KERN_INFO "PinDOWN: PARENT %s.\n", parsec->pathname);
		int i = 0;
		while ((i < MAX_PATHLEN) && (parsec->pathname[i] != '\0')) {
			sec->pathname[i] = parsec->pathname[i];
			i++;
		}
		if (i <= MAX_PATHLEN) {
			sec->pathlen = i;
		}
		else {
			err = -ENOMEM;
			// printk(KERN_INFO "PinDOWN: PATHLEN is too big to store\n");
			goto out;
		}
	}
	p->security = sec;
out:
	return err;

}
static int mysec_security_inode_permission(struct inode *inode, int mask){
    printk(KERN_EMERG "opening sec as %s\n", ((mysec_secinfo*)current->security)->pathname);
    return 0;
}

static struct security_hook_list mysec_hooks[] __ro_after_init = {
    LSM_HOOK_INIT(bprm_check_security, mysec_bprm_check_security),
    LSM_HOOK_INIT(inode_permission, mysec_security_inode_permission),
    LSM_HOOK_INIT(task_alloc, mysec_task_alloc_security),

};
static const struct lsm_id mysec_lsmid = {
	.name = "mysec",
	.id = LSM_ID_MYSEC,
};
static int __init mysec_init(void) {
    printk(KERN_ERR "printing things\n");
    security_add_hooks(mysec_hooks, ARRAY_SIZE(mysec_hooks), &mysec_lsmid);
    return 0;
}
DEFINE_LSM(mysec) = {
    .name = "mysec",
    .init = mysec_init,
};