/**
 * * iterator DP
 */


let nArray = [1, 7, 21, 657, 3, 2, 765, 13, 65]

// @ts-ignore

interface Iterator {
    next(): any;
    hasNext(): boolean;
}

interface Aggregator {
    // @ts-ignore
    createIterator(): Iterator;
}

// @ts-ignore
class ConcreteIterator implements Iterator {
    private collection: any[] = [];
    private position: number = 0;

    constructor(collection: any[]) {
        this.collection = collection;
    }

    public next(): any {
        // Error handling is left out
        var result = this.collection[this.position];
        this.position += 1;
        return result;
    }

    public hasNext(): boolean {
        return this.position < this.collection.length;
    }
}

class Numbers implements Aggregator {
    private collection: number[] = [];

    constructor(collection: number[]) {
        this.collection = collection;
    }

    // @ts-ignore
    public createIterator(): Iterator {
        return new ConcreteIterator(this.collection);
    }
}



let numbers: Numbers = new Numbers(nArray)
let it: ConcreteIterator = <ConcreteIterator>numbers.createIterator();

while (it.hasNext()) {
    console.log(it.next());
}

