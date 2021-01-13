package com.ren.btree;

import java.util.*;

public class BTNode {
    //关键字，以key非降序排序
    private List<Entry> keys;
    //内结点的子节点
    private List<BTNode> childNodes;
    //父节点
    private BTNode parent;

    public BTNode(){
        keys = new LinkedList<Entry>();
        childNodes = new ArrayList<BTNode>();
    }

    public BTNode (BTNode parent){
        this();
        this.parent = parent;
    }

    public BTNode getParentNode(){
        return parent;
    }

    public void setParentNode(BTNode parentNode){
        this.parent = parentNode;
    }

    /**
     * @return 获取键值对列表
     */
    public List<Entry> getKeys(){
        return keys;
    }

    /**
     * 获取键值对
     * @param index
     * @return
     */
    public Entry getKey(int index){
        return keys.get(index);
    }

    public List<BTNode> getChildNodes(){
        return childNodes;
    }

    public BTNode getChild(int index){
        return childNodes.get(index);
    }

    public boolean addChildren(List<BTNode> children){
        for (BTNode node: children){
            node.setParentNode(this);
        }
        childNodes.addAll(children);

        //排序
        Collections.sort(childNodes, new Comparator<BTNode>(){

            public int compare(BTNode o1, BTNode o2) {
                return o1.getKeys().get(0).getKey().compareTo(o2.getKeys().get(0).getKey());
            }
        });

        return true;
    }

    /**
     * 添加键值对
     * @param key
     */
    public void addKey(Entry key){
        SearchResult searchResult = searchKey(this, key.getKey());
        this.getKeys().add(searchResult.getIndex(), key);
    }

    /**
     * 在节点node中搜索关键字key
     * @param node
     * @param key
     * @return
     */
    public SearchResult searchKey(BTNode node, Integer key){
        boolean result = false;
        int index;

        List<Entry> keys = node.getKeys();

        int low = 0;
        int high = keys.size() - 1;
        int mid = 0;
        while (low <= high){
            mid = (low + high) >>> 1;
            Comparable<Integer> midVal = keys.get(mid).getKey();
            int cmp = midVal.compareTo(key);
            if (cmp < 0){
                low = mid + 1;
            }else if(cmp > 0){
                high = mid - 1;
            }else{
                break;
            }
        }
        if (low <= high){   //查找成功
            result = true;
            index = mid;
        }else {
            index = low;
        }

        return new SearchResult(result, index);
    }

    /**
     * 设置父节点并添加孩子结点,添加后需要排序
     * @param child
     * @return
     */
    public boolean addChild(BTNode child){
        child.setParentNode(this);
        childNodes.add(child);
        //排序
        Collections.sort(childNodes, new Comparator<BTNode>() {
            public int compare(BTNode o1, BTNode o2) {
                return o1.getKeys().get(0).getKey().compareTo(o2.getKeys().get(0).getKey());
            }
        });
        return true;
    }

    /**
     * 删除孩子结点
     * @param childNode
     */
    public void removeChild(BTNode childNode){
        childNodes.remove(childNode);
    }

    /**
     * @return 节点中元素个数
     */
    public int sizeOfKeys(){
        return keys.size();
    }

    /**
     * @return 孩子个数
     */
    public int sizeOfChildren(){
        return childNodes.size();
    }
}
