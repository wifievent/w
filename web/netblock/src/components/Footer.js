import { Nav } from "react-bootstrap";
import React from "react";

const Footer = () => {
  return (
    <div>
      <Nav className="footerNav" activeKey="/home">
        <Nav.Item>NetBlock</Nav.Item>
      </Nav>
    </div>
  );
};

export default Footer;
