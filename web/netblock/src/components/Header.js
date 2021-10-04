import { Nav } from "react-bootstrap";

const Header = () => {
  return (
    <div>
      <Nav style={{ backgroundColor: "aqua" }} activeKey="/home">
        <Nav.Item>
          <Nav.Link href="/">NetBlock</Nav.Link>
        </Nav.Item>
        <Nav.Item>
          <Nav.Link href="/about">About</Nav.Link>
        </Nav.Item>
        <Nav.Item>
          <Nav.Link href="/download">Download</Nav.Link>
        </Nav.Item>
      </Nav>
    </div>
  );
};

export default Header;
