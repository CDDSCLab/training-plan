package com.ren.hashJoin;

public class Entry {
    private short num;
    private String value;
    private Entry next;

    public Entry(short num, String value, Entry next) {
        this.num = num;
        this.value = value;
        this.next = next;
    }

    public short getNum() {
        return num;
    }

    public void setNum(short num) {
        this.num = num;
    }

    public String getValue() {
        return value;
    }

    public void setValue(String value) {
        this.value = value;
    }

    public Entry getNext() {
        return next;
    }

    public void setNext(Entry next) {
        this.next = next;
    }
}
