import React, { useReducer } from 'react';
import CartContext from './cart-context';

/**
 * TODO-The goal of this component is simply to manage the cart-Context 
 * TODO-data and provide that context to all components that want access to it.
 **  Get item that should be added to the cart.
 **  Check if the item that the part of the cart (if already have sushi in the cart) i want to
 **  update the existing item
 **  If it not the case i want to add this to the item
 *  TODO-we want to manage this state in this component (the any component effected by context)
 *  TODO-are revaluated whenever the date cart changes
 * 
 ** We define reducer function outside of component function because Reducer function wouldn't need any thing
 ** from CartProvider Function (component function) & it shouldn't be created every time when the component is 
 ** revaluated.
 * 
 * @returns manage data cart in the end
 */

const defaultCartState = {
    items: [],
    totalAmount: 0,
}

const cartReducer = ( state, action ) => {
    if ( action.type === 'ADD_ITEM' ) {
        const updatedTotalAmount = state.totalAmount + action.item.price * action.item.amount
        const existingCartItemIndex = state.items.findIndex( item => item.id === action.item.id )
        const existingCartItem = state.items[ existingCartItemIndex ]
        let updatedItems;
        if ( existingCartItem ) {
            const updatedItem = {
                ...existingCartItem,
                amount: existingCartItem.amount + action.item.amount
            }
            updatedItems = [ ...state.items ]
            updatedItems[ existingCartItemIndex ] = updatedItem
        } else {
            updatedItems = state.items.concat( action.item )
        }
        return {
            items: updatedItems,
            totalAmount: updatedTotalAmount
        }
    }
    if ( action.type === 'REMOVE_ITEM' ) {
        const existingCartItemIndex = state.items.findIndex( item => item.id === action.id )
        const existingItem = state.items[ existingCartItemIndex ]
        const updatedTotalAmount = state.totalAmount - existingItem.price
        let updatedItems;
        if ( existingItem.amount === 1 ) {
            updatedItems = state.items.filter( item => item.id !== action.id )
        } else {
            const updatedItem = { ...existingItem, amount: existingItem.amount - 1 }
            updatedItems = [ ...state.items ]
            updatedItems[ existingCartItemIndex ] = updatedItem
        }

        return {
            items: updatedItems,
            totalAmount: updatedTotalAmount
        }
    }
    return defaultCartState
}

const CartProvider = ( props ) => {
    const [ cartState, dispatchCartAction ] = useReducer( cartReducer, defaultCartState )

    const addItemToCartHandler = item => {
        dispatchCartAction( {
            type: 'ADD_ITEM',
            item: item
        } )
    }
    const removeItemFromCartHandler = id => {
        dispatchCartAction( {
            type: 'REMOVE_ITEM',
            id: id
        } )
    }

    const cartContext = {
        items: cartState.items,
        totalAmount: cartState.totalAmount,
        addItem: addItemToCartHandler,
        removeItem: removeItemFromCartHandler
    }

    return <CartContext.Provider value={ cartContext }>
        { props.children }
    </CartContext.Provider>

};

export default CartProvider;