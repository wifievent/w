import { Nav } from "react-bootstrap";
import { LinkContainer } from "react-router-bootstrap";

const Header = () => {
  return (
    <div>
      <Nav style={{ backgroundColor: "aqua" }} activeKey="/home">
        <Nav.Item>
          <LinkContainer to="/">
            <Nav.Link>Netblock</Nav.Link>
          </LinkContainer>
        </Nav.Item>
        <Nav.Item>
          <LinkContainer to="/about">
            <Nav.Link>About</Nav.Link>
          </LinkContainer>
        </Nav.Item>
        <Nav.Item>
          <LinkContainer to="/download">
            <Nav.Link>Download</Nav.Link>
          </LinkContainer>
        </Nav.Item>
      </Nav>
    </div>
  );
};

export default Header;
