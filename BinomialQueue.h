#ifndef _BINOMIALQUEUE_H
#define _BINOMIALQUEUE_H
#include <vector>
#include <iostream>
#include <exception>
using namespace std;

class UnderflowException: public exception
{
    virtual const char* what() const throw()
    {
        return "Exception thrown: Heap is empty";
    }
};

template <typename Comparable>
class BinomialQueue
{
public:
    BinomialQueue();
    BinomialQueue( const Comparable & item );
    BinomialQueue( const BinomialQueue & rhs );
    ~BinomialQueue();
    
    bool isEmpty() const;
    const Comparable & findMin() const;
    
    void insert( const Comparable & x ){
        
        BinomialQueue singleNode = new BinomialQueue(x);
        merge(singleNode);
        
    }

    void deleteMin();
    
    /**
     * Remove the minimum item and place it in minItem.
     * Throws UnderflowException if empty.
     */
    void deleteMin( Comparable & minItem )
    {
        if ( isEmpty() )
            throw UnderflowException();
        
        int minIndex = findMinIndex(); 
        minItem = theTrees[minIndex]->element;
        BinomialNode *oldRoot = theTrees[ minIndex ];
        BinomialNode *deletedTree = oldRoot -> leftChild;
        delete oldRoot;
        
        // Construct H''
        BinomialQueue deletedQueue;
        deletedQueue.theTrees.resize( minIndex + 1 );
        deletedQueue.currentSize = ( 1 << minIndex ) - 1;
        for( int j = minIndex - 1; j >= 0; j-- )
        {
            deletedQueue.theTrees[j] = deletedTree;
            deletedTree = deletedTree->nextSibling;
            deletedQueue.TheTrees[j]->nextSibling = NULL;
        }
        
        // Construct H'
        theTrees[ minIndex ] = NULL;
        currentSize -= deletedQueue.currentSize + 1;
        
        merge( deletedQueue );
        
    }

    
    void makeEmpty();
    
    /**
     * Merge rhs into the priority queue.
     * rhs becomes empty, rhs must be different from this.
     */
    void merge( BinomialQueue & rhs )
    {
        if( this == &rhs )      //Avoid aliasing problems
            return;
        
        currentSize += rhs.currentSize;
        
        if( currentSize > capacity())
        {
            int oldNumTrees = theTrees.size();
            int newNumTrees = max( theTrees.size(), rhs.theTrees.size()) +1;
            theTrees.resize( newNumTrees );
            for (int i = oldNumTrees; i < newNumTrees; i++ )
                theTrees[i] = NULL;
        }
        
        BinomialNode *carry = NULL;
        for( int i = 0, j = 1; j <= currentSize; i++, j *= 2 )
        {
            BinomialNode *t1 = theTrees[i];
            BinomialNode *t2 = i < rhs.theTrees.size() ? rhs.theTrees[i] : NULL;
            
            int whichCase = t1 == NULL ? 0 : 1;
            whichCase += t2 == NULL ? 0 : 2;
            whichCase += carry == NULL ? 0 : 4;
            
            switch( whichCase )
            {
                case 0: /* No trees */
                case 1: /* Only this */
                    break;
                case 2: /* Only rhs */
                    theTrees[i] = t2;
                    rhs.theTrees[i] = NULL;
                    break;
                case 4: /* Only carry */
                    theTrees[i] = carry;
                    carry = NULL;
                    break;
                case 3: /* this and rhs */
                    carry = combineTrees( t1, t2 );
                    theTrees[i] = rhs.theTrees[i] = NULL;
                    break;
                case 5: /* this and carry */
                    carry = combineTrees( t1, carry );
                    theTrees[i] = NULL;
                    break;
                case 6: /* rhs and carry */
                    carry = combineTrees( t2, carry );
                    rhs.theTrees[i] = NULL;
                    break;
                case 7: /* All three */
                    theTrees[i] = carry;
                    carry = combineTrees( t1, t2 );
                    rhs.theTrees[i] = NULL;
                    break;
            }
        }
        
        for ( int k = 0; k < rhs.theTrees.size(); k++ )
            rhs.theTrees[k] = NULL;
        rhs.currentSize = 0;
        
    }

    
    const BinomialQueue & operator= ( const BinomialQueue & rhs );
    
private:
    struct BinomialNode
    {
        Comparable element;
        int lineNumber;
        BinomialNode *leftChild;
        BinomialNode *nextSibling;
        
        BinomialNode( const Comparable & theElement,
                     BinomialNode *lt, BinomialNode *rt )
        : element( theElement ), leftChild(lt), nextSibling(rt) {}
    };
    
    enum { DEFAULT_TREES = 1 };
    
    int currentSize;                 //Number of items in priority queue
    vector<BinomialNode *> theTrees; //An array of tree roots
    
    /**
     * Find index of tree containing the smalles item in the priority queue.
     * The priority queue must not be empty.
     * Return the index of tree containing the smallest item.
     */
    int findMinIndex() const
    {
        int i;
        int minIndex;
        
        for( i=0; theTrees[i] == NULL; i++ )
            ;
        
        for( minIndex = i; i < theTrees.size(); i++ )
            if( theTrees[i] != NULL &&
               theTrees[i]->element < theTrees[minIndex]->element )
                minIndex = i;
        
        return minIndex;
    }

    int capacity() const;
    
    /**
     * Return the result of mergin equal-sized t1 and t2
     */
    BinomialNode * combineTrees( BinomialNode *t1, BinomialNode *t2 )
    {
        if ( t2->element < t1->element )
            return combineTrees ( t2, t1 );
        t2->nextSibling = t1->leftChild;
        t1->leftChild = t2;
        return t1;
    }    void makeEmpty( BinomialNode * & t );
    BinomialNode * clone( BinomialNode *t ) const;

};

#endif