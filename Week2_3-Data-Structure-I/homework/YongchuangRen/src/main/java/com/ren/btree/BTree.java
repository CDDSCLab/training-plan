package com.ren.btree;

public class BTree {

    private BTNode root;    //BTree的根节点
    private int sizeOfKeys; //BTree中关键字个数

    /**
     * 默认情况下只有一个节点且为叶子结点
     */
    private BTree(){
        //根节点没有父节点
        root = new BTNode(null);
    }

    private static BTree instance = new BTree();

    /**
     * 创建一个BTree
     * @return
     */
    public static BTree newInstance(){
        return instance;
    }

    /**
     * 向BTree中插入关键字
     * 从根节点开始寻找最佳的插入结点
     *      - 若为叶子结点,则先插入key然后判断是否需要分裂
     *      - 若为非叶子结点,则从上到下寻找最佳的叶子结点,然后重复上面叶子结点的情况
     *      * @param key
     * @return
     */
    public boolean insertKey(Entry entry){
        BTNode node = root;
        while (node != null) {
            if (node.sizeOfChildren() == 0){    //叶子结点
                node.addKey(entry);
                if (node.sizeOfKeys() <= Constants.MAX_KEY_SIZE){
                    break;
                }else {     //插入关键字后需要分裂
                    splitNode(node);
                    break;
                }
            }else {     //非叶子节点
                Entry lesser = node.getKey(0);
                if (entry.getKey().compareTo(lesser.getKey()) < 0){ //如果比最小关键字还小
                    node = node.getChild(0);    //则关键字会插入到最左边的子树上
                    continue;
                }
                int size = node.sizeOfKeys();
                int last = size - 1;
                Entry greater = node.getKey(last);
                if (entry.getKey().compareTo(greater.getKey()) > 0){    //比最大的关键字还大
                    node = node.getChild(size);
                    continue;
                }

                //若不属于上述两种情况则需要一步一步查找中间结点
                for (int i = 1; i < node.sizeOfKeys(); i++){
                    Entry prev = node.getKey(i - 1);
                    Entry next = node.getKey(i);
                    if (entry.getKey().compareTo(prev.getKey()) > 0 && entry.getKey().compareTo(next.getKey()) < 0){
                        node = node.getChild(i);
                        break;
                    }
                }
            }
        }
        sizeOfKeys ++;
        return true;
    }

    /**
     * 删除BTree中关键字。先找到关键字在哪个结点中,如果没有找到此结点则直接返回false
     * 如果此结点存在再对该结点分叶子结点和非叶子结点讨论:
     * 对于叶子结点:
     *      1. 既是叶子结点又是根节点且key数不超过最小key数
     *      2. 是叶子结点但不是根节点且key数小于最小key数；这种情况需要合并操作
     *      3. 即是叶子结点又是根节点且key数超过最小key数
     *      4. 是叶子结点但不是根节点且key数超过最小key数
     *  我们其实只需要讨论需不需要合并两种情况
     *  对于非叶子节点：
     *      1. 先获取关键字key在所在的结点中的关键字列表keys中的索引,删除该关键字key
     *      2. 获取并删除左孩子最大结点greatest中的最大的元素,此元素在叶节点中,即用左孩子中最大的关键字填补被删除的关键字key
     *      3. 如果greatest结点中的关键字个数不足则需要合并结点
     *  步骤：
     *  1. 首先查找B树中需删除的元素,如果该元素在B树中存在，则将该元素在其结点中进行删除
     *  2. 如果删除该元素后，首先判断该元素是否有左右孩子结点？
     *      - 如果有，则上移孩子结点中的某相近元素(“左孩子最右边的节点”或“右孩子最左边的节点”)到父节点中，然后是移动之后的情况
     *      - 如果没有，直接删除后，移动之后的情况
     *  3. 删除元素，移动相应元素之后，如果某结点中元素数目（即关键字数）小于ceil(m/2)-1，则需要看其某相邻兄弟结点是否丰满（结点中元素个数大于ceil(m/2)-1
     *      - 如果丰满，则向父节点借一个元素来满足条件；
     *      - 如果其相邻兄弟都刚脱贫，即借了之后其结点数目小于ceil(m/2)-1，则该结点与其相邻的某一兄弟结点进行“合并”成一个结点，以此来满足条件。
     * @param key
     * @return
     */
    public boolean removeKey(Integer key){
        //先找到关键字在哪个结点中，如果找到此节点则直接返回false
        BTNode node = this.findNode(key);
        if(node == null)
            return false;
        int index = 0;
        for (int i = 0; i < node.getKeys().size(); i++){
            //获取key的索引
            if (key.compareTo(node.getKey(i).getKey()) == 0){
                node.getKeys().remove(i);   //删除key所在元素
                index = i;      //记录删除的位置
            }
        }
        if (node.sizeOfChildren() != 0){    //非叶子节点
            BTNode left = node.getChild(index); //获取节点左孩子
            BTNode greatest = this.getGreatestNode(left);
            //获取并删除左孩子最大结点中最大的元素
            Entry replaceValue = greatest.getKeys().remove(greatest.sizeOfKeys() - 1);
            node.addKey(replaceValue);

            //如果删除后导致下溢，需要合并节点
            if (greatest.sizeOfKeys() < Constants.MIN_KEY_SIZE){
                this.combined(greatest);
            }
        }else {     //叶子节点
            if (node.getParentNode() != null && node.sizeOfKeys() < Constants.MIN_KEY_SIZE){
                this.combined(node);
            }else if (node.getParentNode() == null && node.sizeOfKeys() == 0){
                //删除的是树的最后一个元素
                root = null;
            }
        }

        sizeOfKeys--;
        return true;
    }

    /**
     * 从上到下直到叶节点找到最大的叶结点
     * @param node
     * @return
     */
    private BTNode getGreatestNode(BTNode node){
        while (node.sizeOfChildren() != 0){
            node = node.getChild(node.sizeOfChildren() - 1);
        }
        return node;
    }

    /**
     * 此node结点中关键字个数不足，需要合并结点，与splitNode()方法类似，也是从叶节点开始向上递归的
     * 需要分情况讨论:
     *      1. 此结点的右邻居存在且右邻居结点中关键字个数 >= minSize + 1    相当于左旋
     *      2. 此结点的左邻居存在且左邻居结点中关键字个数 >= minSize + 1   相当于右旋
     *      3. 此结点的右邻居存在且父节点关键字个数大于0   结点合并
     *      4. 此结点的左邻居存在且父节点关键字个数大于0  结点合并
     * @param node
     */
    private void combined(BTNode node){
        //先获取此结点的父节点
        BTNode parentNode = node.getParentNode();
        //获取此结点是其父节点中的索引，即第几个孩子
        int index = parentNode.getChildNodes().indexOf(node);
        int indexOfLeftNeighbor = index - 1;
        int indexOfRightNeighbor = index + 1;

        BTNode rightNeighbor = null;
        int rightNeighborSize = 0;
        if (indexOfRightNeighbor < parentNode.sizeOfChildren()) {   //右邻居存在
            rightNeighbor = parentNode.getChild(indexOfRightNeighbor);
            rightNeighborSize = rightNeighbor.sizeOfKeys();
        }
        //右邻居存在且其关键字个数大于最小值
        if (rightNeighbor != null && rightNeighborSize > Constants.MIN_KEY_SIZE){
            //左旋
            Entry removeValue = rightNeighbor.getKeys().remove(0);  //删除右邻居最小元素
            int prev = getIndexOfPreviousValue(parentNode, removeValue.getKey());
            Entry parentValue = parentNode.getKeys().remove(prev);  // 删除父节点中合适位置的元素
            node.addKey(parentValue);   //添加父节点删除的元素到节点中
            parentNode.addKey(removeValue); //将右邻居删除的元素插入父节点中

            if (rightNeighbor.sizeOfChildren() > 0){    //如果右邻居的孩子结点存在，则需要把右邻居的第一个孩子结点删除并添加到node结点中
                node.addChild(rightNeighbor.getChildNodes().remove(0));
            }
        }else {
            BTNode leftNeighbor = null;
            int leftNeighborSize = 0;
            if(indexOfLeftNeighbor >= 0) {//左邻居存在
                leftNeighbor = parentNode.getChild(indexOfLeftNeighbor);
                leftNeighborSize = leftNeighbor.sizeOfKeys();
            }
            //左邻居存在且其关键字个数大于最小值
            if(leftNeighbor != null && leftNeighborSize > Constants.MIN_KEY_SIZE) {//左邻居存在且其关键字个数大于最小值
                //相当于右旋
                Entry removeValue = leftNeighbor.getKeys().remove(leftNeighbor.sizeOfKeys() - 1);
                int next = getIndexOfNextValue(parentNode, removeValue.getKey());
                Entry parentValue = parentNode.getKeys().remove(next);
                node.addKey(parentValue);
                parentNode.addKey(removeValue);

                if (leftNeighbor.sizeOfChildren() > 0){//如果左邻居的孩子结点存在，则需要把左邻居的最后一个孩子结点删除并添加到node结点中
                    node.addChild(leftNeighbor.getChildNodes().remove(leftNeighbor.sizeOfChildren() - 1));
                }
            }else if (rightNeighbor != null && parentNode.sizeOfKeys() > 0){    //右邻居存在且父节点关键字个数大于0
                Entry rightValue = rightNeighbor.getKey(0);//获取右邻居结点中最左边的元素
                int prev = getIndexOfPreviousValue(parentNode, rightValue.getKey());//获取rightValue关键字的父节点中不大于但最接近此关键字的索引
                Entry parentKey = parentNode.getKeys().remove(prev);//在父节点中删除此索引对应的关键字
                parentNode.removeChild(rightNeighbor);//在父节点中删除此索引对应的孩子结点
                node.addKey(parentKey);//将删除的关键字添加到关键字下溢的结点中

                //将右邻居的元素添加进去
                for (int i = 0; i < rightNeighbor.sizeOfKeys(); i++){
                    node.addKey(rightNeighbor.getKey(i));
                }
                //将右邻居的孩子结点也添加进去
                for (int i = 0; i < rightNeighbor.sizeOfChildren(); i++){
                    node.addChild(rightNeighbor.getChild(i));
                }
                if (parentNode.getParentNode() != null && parentNode.sizeOfKeys() < Constants.MIN_KEY_SIZE) {//还没到达根节点
                    this.combined(parentNode);
                }else if (parentNode.sizeOfKeys() == 0){
                    //父节点中没有关键字了，降低树的高度
                    node.setParentNode(null);   //树的高度降低一层，此结点就变为根节点，一定要设置其父节点为null
                    parentNode = null;
                    root = node;
                }
            }else if (leftNeighbor != null && parentNode.sizeOfKeys() > 0){ //左邻居存在且父节点元素个数大于0
                Entry leftValue = leftNeighbor.getKey(leftNeighbor.sizeOfKeys() - 1);//获取左邻居结点中最右边的关键字
                int next = getIndexOfNextValue(parentNode, leftValue.getKey());//获取leftValue关键字的父节点中不小于但最接近此关键字的索引
                Entry parentKey = parentNode.getKeys().remove(next);//在父节点中删除此索引对应的关键字
                parentNode.removeChild(leftNeighbor);//在父节点中删除此索引对应的孩子结点
                node.addKey(parentKey);//将删除的关键字添加到关键字下溢的结点中

                //将左邻居中的关键字添加进去
                for (int i = 0; i < leftNeighbor.sizeOfChildren(); i++){
                    node.addChild(leftNeighbor.getChild(i));
                }
                if (parentNode.getParentNode() != null && parentNode.sizeOfKeys() < Constants.MIN_KEY_SIZE){    //没达到根节点
                    this.combined(parentNode);
                }else if(parentNode.sizeOfKeys() == 0){
                    //父节点中没有关键字，则降低树的高度
                    node.setParentNode(null);
                    parentNode = null;
                    root = node;
                }
            }
        }
    }

    /**
     * 返回node结点中值不大于但最接近与value的关键字的索引，都比value大则返回0，都比value小则返回size-1
     * @param node
     * @param value
     * @return
     */
    private int getIndexOfPreviousValue(BTNode node, Integer value){
        for (int i = 0; i <node.sizeOfKeys(); i++){
            Entry t = node.getKey(i);
            if (t.getKey().compareTo(value) >= 0){
                return i - 1;
            }
        }
        return node.sizeOfKeys() - 1;
    }

    /**
     * 返回node结点中值不小于但最接近与value的关键字的索引，都比value大则返回0，都比value小则返回size-1
     * @param node
     * @param value
     * @return
     */
    private int getIndexOfNextValue(BTNode node, Integer value){
        for (int i = 0; i < node.sizeOfKeys(); i++){
            Entry t = node.getKey(i);
            if (t.getKey().compareTo(value) >= 0) {
                return i;
            }
        }
        return node.sizeOfKeys() - 1;
    }

    /**
     * 从叶子结点开始从下到上进行递归分裂
     * @param node
     */
    private void splitNode(BTNode node){
        //分裂位置发生在size/2，分裂为两个节点
        int splitIndex = node.sizeOfKeys() / 2;
        //需要上移元素
        Entry splitKey = node.getKeys().get(splitIndex);

        //生成新的节点
        BTNode left = new BTNode(null);
        for (int i = 0; i < splitIndex; i++){
            left.addKey(node.getKey(i));
        }
        if (node.sizeOfChildren() > 0){
            left.addChildren(node.getChildNodes().subList(0, splitIndex + 1));
        }

        BTNode right = new BTNode(null);
        for (int i = splitIndex + 1; i < node.sizeOfKeys(); i++){
            right.addKey(node.getKey(i));
        }
        if (node.sizeOfChildren() > 0){
            right.addChildren(node.getChildNodes().subList(splitIndex+1, node.sizeOfChildren()));
        }

        if (node.getParentNode() != null) {     //如果有父节点
            BTNode parent = node.getParentNode();   //获取父节点
            parent.addKey(splitKey);
            parent.removeChild(node);
            parent.addChild(left);
            parent.addChild(right);

            if (parent.sizeOfKeys() > Constants.MAX_KEY_SIZE){
                splitNode(parent);
            }
        }else {     //没有父节点，即该节点是根节点
            BTNode newRoot = new BTNode(null);
            newRoot.getKeys().add(splitKey);
            root = newRoot;
            newRoot.addChild(left);
            newRoot.addChild(right);
        }
    }

    /**
     * 在BTree中查找key的元素，若存在则返回BTNode，不存在则返回null；
     * @param key
     * @return
     */
    public BTNode findNode(Integer key) {
        BTNode node = root;
        while (node != null) {
            if (node.sizeOfChildren() == 0){    //叶子结点
                for (int i = 0; i < node.sizeOfKeys(); i++){
                    if (key == node.getKey(i).getKey()){
                        return node;
                    }
                }
                return null;
            }else { //非叶子节点
                if (key.compareTo(node.getKey(0).getKey()) < 0){    //比最小的小
                    node = node.getChild(0);
                    continue;
                }
                if (key.compareTo(node.getKey(node.sizeOfKeys() - 1).getKey()) > 0){
                    node = node.getChild(node.sizeOfKeys());
                    continue;
                }
                //中间情况
                for (int i = 0; i < node.sizeOfKeys(); i++){
                    if (key.compareTo(node.getKey(i).getKey()) == 0){
                        return node;
                    }
                    if (key.compareTo(node.getKey(i).getKey()) > 0 && key.compareTo(node.getKey(i + 1).getKey()) < 0){
                        node = node.getChild(i + 1);
                        break;
                    }
                }
            }
        }
        return null;
    }

    /**
     * 寻找元素
     * @param index
     * @return
     */
    public String findKey(Integer index){
        BTNode node = findNode(index);
        if (node == null) {
            return "B树中没有该数据";
        }else {
            SearchResult searchResult = node.searchKey(node, index);
            return findNode(index).getKey(searchResult.getIndex()).toString();
        }
    }

    /**
     * @return BTree中关键字个数
     */
    public int sizeOfKeys() {
        return sizeOfKeys;
    }

    public String getTree(BTree tree){
        if (tree.root == null){
            return "此树为空树";
        }
        return getString(tree.root, "*", true);
    }
    private String getString(BTNode node, String prefix, boolean isTail){
        StringBuilder builder = new StringBuilder();

        builder.append(prefix).append((isTail ? "└── " : "├── "));
        for (int i = 0; i < node.sizeOfKeys(); i++){
            Entry value = node.getKey(i);
            builder.append(value.getKey());
            if (i < node.sizeOfKeys() - 1) {
                builder.append(", ");
            }
        }
        builder.append("\n");

        if (node.getChildNodes() != null){
            for (int i = 0; i < node.sizeOfChildren() - 1; i++){
                BTNode obj = node.getChild(i);
                builder.append(getString(obj, prefix + (isTail ? "    " : "│   "), false));
            }
            if (node.sizeOfChildren() >= 1){
                BTNode obj = node.getChild(node.sizeOfChildren() - 1);
                builder.append(getString(obj,prefix + (isTail ? "    " : "│   "), true));
            }
        }
        return builder.toString();
    }
    /**
     * 打印B树
     * @param tree
     * @return
     */
    public String printTree(BTree tree){
        if (tree.root == null){
            return "此树为空树";
        }else {
            StringBuilder builder = new StringBuilder();
            builder = printall(tree.root, builder);
            return builder.toString();
        }
    }

    private StringBuilder printall(BTNode node, StringBuilder builder){
        if (node.sizeOfChildren() != 0) {  //不是叶子节点
            for (int i = 0; i < node.sizeOfChildren(); i++){
                BTNode obj = node.getChild(i);
                printall(obj, builder);
                if (i < node.sizeOfChildren() - 1){
                    builder.append(" " + node.getKeys().get(i).toString());
                }
            }
        }else {
            for (int i = 0; i < node.sizeOfKeys(); i++){
                builder.append(" " + node.getKeys().get(i).toString());
            }
        }
        return builder;
    }
}
