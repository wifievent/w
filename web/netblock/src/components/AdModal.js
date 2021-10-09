import React from "react";
import AdYoutube from "./AdYoutube";
import { Modal, Button } from "react-bootstrap";

const AdModal = ({ show, handleClose }) => {
  return (
    <div>
      <Modal size="lg" show={show} onHide={handleClose}>
        <Modal.Header closeButton>
          <Modal.Title>Download NetBlock and Support us</Modal.Title>
        </Modal.Header>
        <Modal.Body>
          <AdYoutube />
        </Modal.Body>
        <Modal.Footer>
          <Button variant="secondary" onClick={handleClose}>
            Close
          </Button>
        </Modal.Footer>
      </Modal>
    </div>
  );
};

export default AdModal;
