package com.ren.btree;

public class Entry {
    private Integer key;
    private String value;

    public Entry(Integer key, String value) {
        this.key = key;
        this.value = value;
    }

    public Integer getKey() {
        return key;
    }

    public Entry setKey(Integer key) {
        this.key = key;
        return this;
    }

    public String getValue() {
        return value;
    }

    public Entry setValue(String value) {
        this.value = value;
        return this;
    }

    @Override
    public String toString() {
        return "Entry{" +
                "key=" + key +
                ", value='" + value + '\'' +
                '}';
    }
}
