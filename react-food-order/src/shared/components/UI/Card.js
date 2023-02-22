import React from 'react';
import classes from './Card.module.css'

/**
 * 
 * @param {children} props 
 * @returns 
 */

const Card = ( props ) => {
    return <div className={ classes.card }>{ props.children } </div>;
};

export default Card;