## 1.并行环境优化功能——设置进程CPU亲和度

功能描述：在分布式数据库服务的应用场景下，需要在同一台服务器上部署多个数据库节点，但是有些平台的处理器对多任务并发的设计比较差，导致系统资源占用率较低，为此可以通过将不同worker数据库的子进程绑定到某个cpu group的方式，来节约数据库服务进程在不同cpu group之间切换的代价。

编译参数：--with-cpu_affinity

GUC参数：

| 参数名称      | 参数说明                            | 参数范围 | 默认值 |
| ------------- | ----------------------------------- | -------- | ------ |
| cpu_affinity  | 用于限定CPU亲和度设置功能是否开启， | on/off   | off    |
| cpu_groups    | 将机器的所有CPU一共划分为多少组。   | 1-99     | 1      |
| cpu_group_num | 将当前server绑定到哪一个分组中      | 1-99     | 1      |

其他：需要注意的是，cpu_groups的分组数，一般要小于等于分布式节点的个数。否则会导致一部分的CPU核心无法参与运算。

**对应函数**：

### 1.```pg_get_cpu_affinity();``` [显示数据库进程运行在那个CPU socket分组上]  （cpu_affinity列为当前pid的亲和度设置情况）

```sql
postgres=# select * from  pg_get_cpu_affinity();
 datid |  pid  | usesysid | application_name | state | query | wait_event_type | wait_event | xact_start | query_start |         backend_start         |         state_change          | client_addr | client_hostname | client_port | backend_xid | backend_xmin |  backend_type  | ssl | sslversion | sslcipher | sslbits | ssl_client_dn | ssl_client_serial | ssl_issuer_dn | gss_auth | gss_princ | gss_enc | leader_pid | query_id | cpu_affinity 
-------+-------+----------+------------------+-------+-------+-----------------+------------+------------+-------------+-------------------------------+-------------------------------+-------------+-----------------+-------------+-------------+--------------+----------------+-----+------------+-----------+---------+---------------+-------------------+---------------+----------+-----------+---------+------------+----------+--------------
     5 | 40366 |       10 | psql             | idle  |       | Client          | ClientRead |            |             | 2022-11-23 17:50:23.705498+08 | 2022-11-23 17:50:23.711359+08 |             |                 |          -1 |             |              | client backend | f   |            |           |         |               |                   |               | f        |           | f       |            |          | 0-23
(1 row)


```

### 2.```pg_get_cpu_core()``` [显示数据库服务进程此刻正在哪个CPU核心上运行，**默认屏蔽一些后台常驻服务，仅显示后派生的服务进程。**]   (返回当前数据库所有后台server进程此刻正在运行在CPU哪个核心中，**此函数需要被GUI前端反复调用，用作刷新显示**)

```sql
postgres=# select * from pg_get_cpu_core();
  pid   | cpu_core 
--------+----------
 40366  |       4
 128789 |       21
(2 rows)


```

### 3.显示guc参数

```sql
postgres=# show cpu_affinity ;
 cpu_affinity 
--------------
 on
(1 row)

postgres=# show cpu_groups;
 cpu_groups 
------------
 8
(1 row)

postgres=# show cpu_group_num;
 cpu_group_num 
---------------
 1
(1 row)

```
