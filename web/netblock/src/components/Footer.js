import { Nav } from "react-bootstrap";
import React from "react";

const Footer = () => {
  return (
    <div>
      <Nav style={{ backgroundColor: "aqua" }} activeKey="/home">
        <Nav.Item>NetBlock</Nav.Item>
      </Nav>
    </div>
  );
};

export default Footer;
