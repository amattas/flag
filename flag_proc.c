#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/random.h>
#include <linux/seq_file.h>

#define PROC_NAME "flag"
#define LINE_COUNT 16
#define BYTES_PER_LINE 32

static struct proc_dir_entry *flag_entry;

static void emit_random_line(struct seq_file *m)
{
    u8 raw[BYTES_PER_LINE];
    char hex[BYTES_PER_LINE * 2 + 1];
    int i;

    get_random_bytes(raw, sizeof(raw));

    for (i = 0; i < BYTES_PER_LINE; ++i)
        snprintf(hex + (i * 2), 3, "%02X", raw[i]);

    hex[sizeof(hex) - 1] = '\0';
    seq_printf(m, "%s\n", hex);
}

static int flag_proc_show(struct seq_file *m, void *v)
{
    int i;

    for (i = 0; i < LINE_COUNT; ++i)
        emit_random_line(m);

    return 0;
}

static int flag_proc_open(struct inode *inode, struct file *file)
{
    return single_open(file, flag_proc_show, NULL);
}

static const struct proc_ops flag_proc_ops = {
    .proc_open = flag_proc_open,
    .proc_read = seq_read,
    .proc_lseek = seq_lseek,
    .proc_release = single_release,
};

static int __init flag_proc_init(void)
{
    flag_entry = proc_create(PROC_NAME, 0444, NULL, &flag_proc_ops);
    if (!flag_entry)
        return -ENOMEM;

    pr_info("/proc/%s created\n", PROC_NAME);
    return 0;
}

static void __exit flag_proc_exit(void)
{
    proc_remove(flag_entry);
    pr_info("/proc/%s removed\n", PROC_NAME);
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Codex");
MODULE_DESCRIPTION("Proc flag random data module");

module_init(flag_proc_init);
module_exit(flag_proc_exit);
