

class client {
    public color: string
    public model: string
    constructor(color: string, model: string) {
        this.color = color;
        this.model = model;
    }
    getColor(): string {
        return this.color;
    }
    getModel(): string {
        return this.model;
    }
}

interface Factory {
    createProduct(color: string, model: string): client;
}

class Honda implements Factory {
    createProduct(color: string, model: string): client {
        return new client(color, model);
    }
}
class Toyota implements Factory {
    createProduct(color: string, model: string): client {
        return new client(color, model);
    }
}
class Suzuki implements Factory {
    createProduct(color: string, model: string): client {
        return new client(color, model);
    }
}

const factory = new Honda();
const productA = factory.createProduct('red', 'Honda');
console.log(productA.getColor());