// interface IShape {
//     Draw(): void;
// }
// class Rectangle implements IShape {
//     private width: number = 0;
//     private height: number = 0


//     set Height(v: number) {
//         this.height = v;
//     }
//     set Width(v: number) {
//         this.width = v;
//     }

//     get Height(): number {
//         return this.height;
//     }

//     get Width(): number {
//         return this.width;
//     }

//     public Draw(): void {
//         console.log("Rectangle");
//     }
// }

// class AdvanceRectangle extends Rectangle {
//     public Area() {
//         return (this.Width * this.Height);
//     }
// }

// function Main() {
//     try {
//         let rectangle = new AdvanceRectangle();
//         rectangle.Height = 10;
//         rectangle.Width = 20;
//         console.log(rectangle.Area());
//     } catch (error) {
//         console.log(error);
//     }
// }
// Main()