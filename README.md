# memcachedStudy
memcachedStudy
# 一致性哈希实现分布式缓存
##待完成
# memcached 与 redis 区别

## 集群：
redis 和memcached都支持集群

## 数据类型
Redis支持的数据类型要丰富得多,Redis不仅仅支持简单的k/v类型的数据，同时还提供String，List,Set,Hash,Sorted Set,pub/sub,Transactions数据结构的存储。其中Set是HashMap实现的，value永远为null而已

memcache支持简单数据类型，需要客户端自己处理复杂对象 k/v

## 持久性
redis支持数据落地持久化存储,可以将内存中的数据保持在磁盘中，重启的时候可以再次加载进行使用。

memcache不支持数据持久存储

## 分布式存储
redis支持master-slave复制模式

memcache可以使用一致性hash做分布式

## value大小不同
memcache是一个内存缓存，key的长度小于250字符**，单个item存储要小于1M**，不适合虚拟机使用

## 数据一致性不同
Redis只使用单核，而Memcached可以使用多核，所以平均每一个核上Redis在存储小数据时比Memcached性能更高。而在100k以上的数据中，Memcached性能要高于Redis，虽然Redis最近也在存储大数据的性能上进行优化，但是比起Memcached，还是稍有逊色。

redis使用的是单线程模型，保证了数据按顺序提交。

memcache需要使用cas保证数据一致性。CAS（Check and Set）是一个确保并发一致性的机制，属于“乐观锁”范畴；原理很简单：拿版本号，操作，对比版本号，如果一致就操作，不一致就放弃任何操作

## cpu利用
redis单线程模型只能使用一个cpu，可以开启多个redis进程

# 关于redis
## （1）什么是redis? /功能点/优势点/缺点
基于内存的高性能数据库/纯内存操作，性能出色/每秒可以处理超过10万次读写操作/

支持多种数据结构/单个value最大限制1GB/

因内存数据库/收到物理内存限制/不能做海量数据高性能读写

## （2）支持的数据类型
Redis通过Key-Value的单值不同类型来区分, 以下是支持的类型: Strings Lists Sets 求交集、并集(通过HashMap实现，是个Value永远为空的HashMap) Sorted Set (使用了HashMap 和SkipList,利用HashMap中的score进行排序) hashes

## （3）为什么redis需要把所有数据放到内存中？
Redis为了达到最快的读写速度将数据都读到内存中，并通过异步的方式将数据写入磁盘。所以redis具有快速和数据持久化的特征。如果不将数据放在内存中，磁盘I/O速度为严重影响redis的性能。在内存越来越便宜的今天，redis将会越来越受欢迎。 如果设置了最大使用的内存，则数据已有记录数达到内存限值后不能继续插入新值。

## （4）Redis是单进程单线程的
redis利用队列技术将并发访问变为串行访问，消除了传统数据库串行控制的开销

## （5）虚拟内存
当你的key很小而value很大时,使用VM的效果会比较好.因为这样节约的内存比较大. 当你的key不小时,可以考虑使用一些非常方法将很大的key变成很大的value,比如你可以考虑将key,value组合成一个新的value. vm-max-threads这个参数,可以设置访问swap文件的线程数,设置最好不要超过机器的核数,如果设置为0,那么所有对swap文件的操作都是串行的.可能会造成比较长时间的延迟,但是对数据完整性有很好的保证.

自己测试的时候发现用虚拟内存性能也不错。如果数据量很大，可以考虑分布式或者其他数据库

## （6）分布式
redis支持主从的模式。原则：Master会将数据同步到slave，而slave不会将数据同步到master。Slave启动时会连接master来同步数据。

这是一个典型的分布式读写分离模型。我们可以利用master来插入数据，slave提供检索服务。这样可以有效减少单个机器的并发访问数量

## （7）读写分离模型
通过增加Slave DB的数量，读的性能可以线性增长。为了避免Master DB的单点故障，集群一般都会采用两台Master DB做双机热备，所以整个集群的读和写的可用性都非常高。 读写分离架构的缺陷在于，不管是Master还是Slave，每个节点都必须保存完整的数据，如果在数据量很大的情况下，集群的扩展能力还是受限于单个节点的存储能力，而且对于Write-intensive类型的应用，读写分离架构并不适合。
