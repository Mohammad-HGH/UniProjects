//? abstract factory design pattern example


class Chair {
    onSit() {
        console.log("chair is settable");
    }

    haveLegs() {
        console.log("chair has legs");
    }
}

class CoffeeTable {
    onSit() {
        console.log("coffee table is settable");
    }

    haveLegs() {
        console.log("coffee table has legs");
    }
}

interface FurnitureFactory {
    createChair(): Chair;

    createTable(): CoffeeTable;
}

interface VictorianFurnitureFactory extends FurnitureFactory {
    createChair(): Chair;

    createTable(): CoffeeTable;
}

interface ModernFurnitureFactory extends FurnitureFactory {
    createChair(): Chair;

    createTable(): CoffeeTable;
}

interface ClassicFurnitureFactory extends FurnitureFactory {
    createChair(): Chair;

    createTable(): CoffeeTable;
}

const buyVictorianFurniture: VictorianFurnitureFactory = {
    createChair(): Chair {
        return new Chair();
    },
    createTable(): CoffeeTable {
        return new CoffeeTable();
    }
}

buyVictorianFurniture.createChair().onSit();