import React, { Fragment } from 'react';
import ReactDOM from 'react-dom';
import classes from './Modal.module.css'

/**
 * TODO - in this section we use React Portal to render modal
 * TODO - modal show in popup in certain window and contains:
 * 1- 
 * 2- 
 * @param {*} props 
 * @returns 
 */

const Backdrop = props => {
    return <div className={ classes.backdrop } onClick={ props.onClose } />
}

const ModalNotify = props => {
    return <div className={ classes.modal }>
        <div className={ classes.content }>{ props.children }</div>
    </div>
}

const portalElement = document.getElementById( 'notification' )

const Modal = ( props ) => {
    return (
        <Fragment>
            { ReactDOM.createPortal( <Backdrop onClose={ props.onClose } />, portalElement ) }
            { ReactDOM.createPortal( <ModalNotify>{ props.children }</ModalNotify>, portalElement ) }
        </Fragment>
    );
};

export default Modal;