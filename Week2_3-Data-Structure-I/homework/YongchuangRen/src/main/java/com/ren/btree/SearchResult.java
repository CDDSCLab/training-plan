package com.ren.btree;

/**
 * 在B树节点中搜索给定键值的返回结果
 * 该结果由两部分组成。第一部分表示此次查找是否成功
 *      - 如果查找成功,第二部分表示给定键值在B树节点中的位置
 *      - 如果查找失败,第二部分表示给定键值应该插入的位置
 */
public class SearchResult {

    private boolean result;
    private int index;

    public SearchResult(boolean result, int index) {
        super();
        this.result = result;
        this.index = index;
    }

    /**
     * @return 给定键值在B树节点中的位置或者给定键值应该插入的位置
     */
    public int getIndex() {
        return index;
    }

    /**
     * @return 查找是否成功
     */
    public boolean getResult(){
        return result;
    }
}
