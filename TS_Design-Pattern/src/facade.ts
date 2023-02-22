interface Shape {
    draw(height: number, width: number): void;
}

class Rectangle implements Shape {
    /**
     * draws the rectangle
     */
    public draw(height: number, width: number): void {
        // draw rectangle
        console.log(`draw rectangle with height ${height} and width ${width}`);

    }
}

class Square implements Shape {
    public draw(height: number, width: number): void {
        // draw square
        console.log(`draw square with height ${height} and width ${width}`);
    }
}

class Circle implements Shape {
    public draw(height: number, width: number): void {
        // draw circle
        console.log(`draw circle with height ${height} and width ${width}`);
    }
}


class ClientCode {
    protected static width: number;
    protected static height: number;
    public static drawShape(width: number, height: number) {
        height = height;
        width = width;

        this.drawRectangle(height, width);
        this.drawSquare(height, width);
        this.drawCircle(height, width);
    }

    private static drawRectangle(height: number, width: number) {
        let rectangle = new Rectangle();
        rectangle.draw(height, width);  // draw rectangle
    }
    private static drawSquare(height: number, width: number) {
        let square = new Square();
        square.draw(height, width);  // draw square
    }
    private static drawCircle(height: number, width: number) {
        let circle = new Circle();
        circle.draw(height, width);  // draw circle
    }
}

ClientCode.drawShape(10, 20);