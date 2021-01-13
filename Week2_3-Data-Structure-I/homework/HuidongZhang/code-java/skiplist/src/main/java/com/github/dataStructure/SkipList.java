package com.github.dataStructure;

import java.util.Random;

public class SkipList<V> {
    
    private int HEAD_KEY = Integer.MIN_VALUE;
    private int TAIL_KEY = Integer.MAX_VALUE;
    private double PROBABILITY = 0.5;
    private Node<V> head;
    private Node<V> tail;
    private Random random;
    private int currMaxLevel;

    public SkipList() {
        head = new Node<V>(HEAD_KEY, null);
        tail = new Node<V>(TAIL_KEY, null);
        head.next = tail;
        tail.pre = head;
        currMaxLevel = 0;
        random = new Random();
    }

    //Verdict The Existence Of The Desired Key
    public Boolean search(int key) {
        Node<V> currNode = head;
        while(currNode != null) {
            if(currNode.key == key) {
                return true;
            }
            else if(currNode.key < key) {
                currNode = currNode.next;
            }
            else {
                currNode = currNode.down;
            }

        }
        return false;
    }

    //Find The Node Before The Desired Key(Exists Or Not)
    public Node<V> searchNode(int key) {
        Node<V> currNode = head;
        while(true) {
            while(currNode.next != null && currNode.next.key <= key) {
                currNode = currNode.next;
            }
            if(currNode.down != null) {
                currNode = currNode.down;
            }
            else {
                break;
            }
        }
        return currNode;
    }
    
    public V insert(int key, V value) {
        Node<V> currNode = searchNode(key);

        if(currNode == null) {
            throw new RuntimeException("Search Node Null.");
        }
        //Update The Matching Value And Return
        if(currNode.key == key) {
            currNode.value = value;
            return value;
        }

        //Start Inserting From The Bottom Level
        Node<V> newNode = new Node<V>(key, value);
        insertNode(currNode, newNode);

        int currLevel = 0;
        while(random.nextDouble() > PROBABILITY) {
            if(currLevel >= currMaxLevel) {
                addEmptyLevel();
            }

            //Search For The Former Node In The Upper Level
            while(currNode.up == null) {
                currNode = currNode.pre;
            }
            currNode = currNode.up;

            Node<V> newUpperNode = new Node<V>(key, value);
            insertNode(currNode, newUpperNode);
            newUpperNode.down = newNode;
            newNode.up = newUpperNode;
            newNode = newUpperNode;
            currLevel++;
        }
        return value;
    }

    public V erase(int key) {
        Node<V> currNode = searchNode(key);
        if(currNode.key != key) {
            return null;
        }
        V res = currNode.value;
        Node<V> nextNode;
        while(currNode != null) {
            nextNode = currNode.next;
            nextNode.pre = currNode.pre;
            currNode.pre.next = nextNode;
            //Move The Higher Level, Repeat Erasing
            currNode = currNode.up;
        }
        return res;
    }

    public void addEmptyLevel() {
        Node<V> newHead = new Node<V>(HEAD_KEY, null);
        Node<V> newTail = new Node<V>(TAIL_KEY, null);
        newHead.next = newTail;
        newHead.down = head;
        newTail.pre = newHead;
        newTail.down = tail;
        head.up = newHead;
        tail.up = newTail;
        head = newHead;
        tail = newTail;
        currMaxLevel++;
    }

    private void insertNode(Node<V> currNode, Node<V> newNode) {
        newNode.next = currNode.next;
        newNode.pre = currNode;
        currNode.next.pre = newNode;
        currNode.next = newNode;
    }
}