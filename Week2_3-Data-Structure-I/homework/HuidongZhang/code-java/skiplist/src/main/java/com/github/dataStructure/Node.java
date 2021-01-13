package com.github.dataStructure;

public class Node<V> {
    int key;
    V value;
    Node<V> next;
    Node<V> pre;
    Node<V> down;
    Node<V> up;

    public Node(int key, V value) {
        this.key = key;
        this.value = value;
    }
}
