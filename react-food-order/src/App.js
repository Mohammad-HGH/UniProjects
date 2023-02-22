import './App.css'
import { useState } from "react";
import Header from "./shared/components/layout/Header";
import Meals from "./shared/components/meals/Meals";
import Cart from './shared/components/cart/Cart';
import CartProvider from './shared/context/CartProvider';
import { Scrollbar } from 'smooth-scrollbar-react';


function App () {
    const [ cartIsShown, setCartIsShown ] = useState( false )

    const showCartHandler = () => {
        setCartIsShown( true )
    }
    const hideCartHandler = () => {
        setCartIsShown( false )
    }


    return (
        <Scrollbar
            plugins={ {
                overscroll: {
                    effect: 'bounce',
                },
            } }>
            <div className='list-data'>
                <CartProvider>
                    { cartIsShown && <Cart onClose={ hideCartHandler } /> }
                    <Header onShowCart={ showCartHandler } />
                    <main>
                        <Meals />
                    </main>
                </CartProvider>
            </div>
        </Scrollbar>
    );
}

export default App;
