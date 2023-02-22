import React, { forwardRef } from 'react';
import classes from './Input.module.css'

/*
 * Pass each key & value as props to input by Spread Operator
 * @param {input.id,label} props 
 * @returns 
*/


const Input = forwardRef( ( props, ref ) => {
    return (
        <div className={ classes.input }>
            <label htmlFor={ props.input.id }>{ props.label }</label>
            <input ref={ ref } type="number" { ...props.input } />
        </div>
    );
} )

export default Input;