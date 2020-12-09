package com.ren.btree;

public class Constants {
    /**
     * BTree的阶
     * BTree中关键字个数为[ceil(m/2)-1,m-1]
     * BTree中子树个数为[ceil(m/2),m]
     */
    public static final int BTREE_ORDER = 4;
    //非根节点中最小的关键字个数
    public static final int MIN_KEY_SIZE = (int)(Math.ceil(Constants.BTREE_ORDER / 2.0) - 1);
    //非根节点中最大的关键字个数
    public static final int MAX_KEY_SIZE = Constants.BTREE_ORDER - 1;
}
