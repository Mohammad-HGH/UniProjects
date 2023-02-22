import React, { createContext } from 'react';

/**
 * Context generally created
 */

const CartContext = createContext( {
    items: [],
    totalAmount: 0,
    addItem: ( item ) => { },
    removeItem: ( id ) => { }
} )

export default CartContext