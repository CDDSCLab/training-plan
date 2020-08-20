package fanfan;

import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * @author ZhouFan
 * @ClassName BloomFilterEnhance
 * @Description 布隆过滤器的Java实现类，带删除功能
 */
public class BloomFilterEnhance{
	
	 
	private static int  BLOOM_HASH_TABLE_SIZE;//哈希表的大小
	private final int[] seeds;//哈希表的种子
	private final AtomicInteger useCount = new AtomicInteger();
	private final Double autoClearRate;//自动清空概率
	//布隆过滤器的哈希表
    private Map<Integer, Integer> bloomFilterCountMap = new HashMap<>();
    
		//自动清空过滤器内部信息的使用比率，传null则表示不会自动清理;
		//当过滤器使用率达到100%时，则无论传入什么数据，都会认为在数据已经存在了;
		//当希望过滤器使用率达到80%时自动清空重新使用，则传入0.8
		public  BloomFilterEnhance(MisjudgmentRate rate, int dataCount, Double autoClearRate){
			
		   int  BLOOM_HASH_TABLE_SIZE = rate.seeds.length * dataCount;
		         System.out.println("哈希表长为"+BLOOM_HASH_TABLE_SIZE);
			 if(BLOOM_HASH_TABLE_SIZE<0 || BLOOM_HASH_TABLE_SIZE>Integer.MAX_VALUE){
				throw new RuntimeException("位数太大溢出了，请降低误判率或者降低数据大小");
			}
			seeds = rate.seeds;
			this.autoClearRate = autoClearRate;
			BloomFilterEnhance.BLOOM_HASH_TABLE_SIZE= BLOOM_HASH_TABLE_SIZE;
			for (int i = 0; i < BLOOM_HASH_TABLE_SIZE; i++) {
	            bloomFilterCountMap.put(i, 0);
	        }
		}
		//获取hashcode
		private int hash(String data, int seeds) {
			char[] value = data.toCharArray();
			int hash = 0;
			if(value.length>0){
				for(int i=0; i<value.length; i++){
					hash = hash + seeds*value[i];
				}
			}
			hash = hash % BLOOM_HASH_TABLE_SIZE;
			return Math.abs(hash);
		}
    
    public enum MisjudgmentRate {
		// 这里要选取质数，能很好的降低错误率
	
		VERY_SMALL(new int[] {3}),
		
		SMALL(new int[] {15,77}), //
		
		MIDDLE(new int[] { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53 }), //
		
		HIGH(new int[] {  59, 61, 67, 71, 73, 79, 83, 89, 97,101, 103, 107, 109, 113, 127, 131 });
		
		private int[] seeds;
		
		//枚举类型MIDDLE构造函数将seeds数组初始化
		private MisjudgmentRate(int[] seeds) {

			
			this.seeds = seeds;
		}
		
		public int[] getSeeds() {
			return seeds;
		}
		
		public void setSeeds(int[] seeds) {
			this.seeds = seeds;
		}
	}

    /**
     * 布隆过滤器：新增操作
     */
    public void add(String element) {
    	//是否需要清理
    	checkNeedClear();
    	//计算每种哈希函数得到的哈希值
    	
    	for(int i=0; i<seeds.length; i++)
    	{
			//计算位hash值
    		int bloomHashCode = hash(element, seeds[i]); 
            //加入到布隆过滤器"CountMap"之中
            bloomFilterCountMap.put(bloomHashCode, bloomFilterCountMap.get(bloomHashCode) + 1);

    	}
    }
    //清空过滤器
	private void checkNeedClear() {
		if(autoClearRate != null){
			if(getUseRate() >= autoClearRate){
				synchronized (this) {
					if(getUseRate() >= autoClearRate){
						bloomFilterCountMap.clear();
						useCount.set(0);
					}
				}
			}
		}
	}
	//得到当前过滤器的使用率
	private Double getUseRate() {
		return (double)useCount.intValue()/(double)BLOOM_HASH_TABLE_SIZE;
	}
    /**
     * 布隆过滤器：查询是否存在（有一定的误判率，跟哈希表大小和哈希函数有关）
     */
    public boolean exists(String element) {
        //计算每种哈希函数得到的哈希值
    	for(int i=0; i<seeds.length; i++)
    	{
			//计算位hash值
    		int bloomHashCode = hash(element, seeds[i]); 
            //获取hashcode
            if(bloomFilterCountMap.get(bloomHashCode) <= 0) 
                  return false;
    	}
    	return true;
    	
    }
        

    /**
     * 布隆过滤器：删除某个元素（必须是原先add()过的元素，否则有一定风险）
     */
    public boolean delete(String element) {
        //如果需要删除的element不存在，返回false
        if (!exists(element)) {
            return false;
        }
      //计算每种哈希函数得到的哈希值
    	for(int i=0; i<seeds.length; i++)
    	{
			//计算位hash值
    		int bloomHashCode = hash(element, seeds[i]); 
    		bloomFilterCountMap.put(bloomHashCode, bloomFilterCountMap.get(bloomHashCode) - 1);
    	}
        
        //删除成功
        return true;
    }

    /**
     * 运行验证布隆过滤器
     */
    public static void main(String[] args) {
        //布隆过滤器类
        BloomFilterEnhance bloomFilterEnhance = new BloomFilterEnhance(MisjudgmentRate.MIDDLE,50, 0.8);
        int j=0;
        //加入0-100000的数据
		for(int i=0;i<100000;i++) {
			bloomFilterEnhance.add(Integer.toString(i));
		}
		//测试100000-200000的数据
		for(int i=100000;i<2000000;i++) {
			if(bloomFilterEnhance.exists(Integer.toString(i)))
				{
				System.out.println("第"+i+"个数冲突了");
				j++;
				}
		}
		System.out.println("总共有"+j+"个数冲突了");
	
	}

    
}
