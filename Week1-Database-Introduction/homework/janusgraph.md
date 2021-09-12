## 一、安装janusgraph

参考https://docs.janusgraph.org/getting-started/installation/

需要先在docker中启动服务器并实例化，然后再为客户端启动容器

`docker run --name janusgraph-default janusgraph/janusgraph:latest`

`docker run --rm --link janusgraph-default:janusgraph -e GREMLIN_REMOTE_HOSTS=janusgraph \    -it janusgraph/janusgraph:latest ./bin/gremlin.sh`

#客户端连接的是--name服务器的名字

## 二、运行官方示例

加载诸神图：

`graph = JanusGraphFactory.open('conf/janusgraph-inmemory.properties')`

`GraphOfTheGodsFactory.loadWithoutMixedIndex(graph, true)`

#使用这种方式不需要配置index后端

在图数据库中访问数据最好的方式是先找一个入口：

该入口可以是点或边

`gremlin> saturn = g.V().has('name', 'saturn').next()`

`==>v[4136]`

通过valueMap查看saturn的属性：

`gremlin> g.V(saturn).valueMap()`
`==>[name:[saturn],age:[10000]]`

通过图中的关系查找saturn的孙子：

`gremlin> g.V(saturn).in('father').in('father').values('name')`
`==>hercules`

通过边属性查看发生在Athens周围50km的所有事件：

`gremlin>g.E().has('place', geoWithin(Geoshape.circle(37.97, 23.72, 50)))`

`==>e[5j5-360-7x1-6go][4104-battled->8376]`
`==>e[5xd-360-7x1-9ig][4104-battled->12328]`

`gremlin> g.V(4104).valueMap()`
`==>[name:[hercules],age:[30]]`
`gremlin> g.V(8376).valueMap()`
`==>[name:[nemean]]`
`gremlin> g.V(12328).valueMap()`
`==>[name:[hydra]]`

图索引（janusgraph会自动选择为边或点）：

`gremlin> g.E().has('place', geoWithin(Geoshape.circle(37.97, 23.72, 50))).as('source').inV().as('god2').select('source').outV().as('god1').select('god1', 'god2').by('name')`

`==>[god1:hercules,god2:nemean]`
`==>[god1:hercules,god2:hydra]`

使用loop简化查询：

`gremlin> hercules = g.V(saturn).repeat(__.in('father')).times(2).next()`
`==>v[4104]`

查询hercules父母的属性：

`gremlin> g.V(hercules).out('father', 'mother')`
`==>v[4152]`
`==>v[8232]`
`gremlin> g.V(hercules).out('father', 'mother').values('name')`
`==>jupiter`
`==>alcmene`
`gremlin> g.V(hercules).out('father', 'mother').label()`
`==>god`
`==>human`
`gremlin> hercules.label()`
`==>demigod`

查询hercules参与的战争：

`gremlin> g.V(hercules).out('battled')`
`==>v[8376]`
`==>v[12296]`
`==>v[12328]`
`gremlin> g.V(hercules).out('battled').valueMap()`
`==>[name:[nemean]]`
`==>[name:[cerberus]]`
`==>[name:[hydra]]`
`gremlin> g.V(hercules).outE('battled').has('time', gt(1)).inV().values('name')`
`==>cerberus`
`==>hydra`
`gremlin> g.V(hercules).outE('battled').has('time', gt(1)).inV().values('name').toString()`
`==>[GraphStep(vertex,[v[4104]]), VertexStep(OUT,[battled],edge), HasStep([time.gt(1)]), EdgeVertexStep(IN), PropertiesStep([name],value)]`

一些查询实例：

`gremlin> pluto = g.V().has('name', 'pluto').next()`
`==>v[8200]`
`gremlin> g.V(pluto).out('lives').in('lives').values('name')`
`==>pluto`
`==>cerberus`
`gremlin>  g.V(pluto).out('lives').in('lives').where(is(neq(pluto))).values('name')`
`==>cerberus`
`gremlin> g.V(pluto).as('x').out('lives').in('lives').where(neq('x')).values('name')`
`==>cerberus`



`gremlin>  g.V(pluto).out('brother').out('lives').values('name')`
`==>sky`
`==>sea`
`gremlin> g.V(pluto).out('brother').as('god').out('lives').as('place').select('god', 'place')`
`==>[god:v[4152],place:v[4256]]`
`==>[god:v[4280],place:v[4232]]`
`gremlin> g.V(pluto).out('brother').as('god').out('lives').as('place').select('god', 'place').by('name')`
`==>[god:jupiter,place:sky]`
`==>[god:neptune,place:sea]`



`gremlin> g.V(pluto).outE('lives').values('reason')`
`==>no fear of death`
`gremlin> g.E().has('reason', textContains('loves'))`
`08:57:14 WARN  org.janusgraph.graphdb.transaction.StandardJanusGraphTx  - Query requires iterating over all vertices [(reason textContains loves)]. For better performance, use indexes`
`==>e[2dj-37c-9hx-3a8][4152-lives->4256]`
`==>e[36f-3aw-9hx-39k][4280-lives->4232]`

`gremlin> g.E().has('reason', textContains('loves')).as('source').values('reason').as('reason').select('source').outV().values('name').as('god').select('source').inV().values('name').as('thing').select('god', 'reason', 'thing')`

`==>[god:jupiter,reason:loves fresh breezes,thing:sky]`
`==>[god:neptune,reason:loves waves,thing:sea]`

## TODO：

1.实现一些更复杂的查询，感受一下janusgraph的神奇
