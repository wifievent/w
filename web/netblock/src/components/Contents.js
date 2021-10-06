import React from "react";
import { Carousel } from "react-bootstrap";
import card1 from "../img/01.png";
import card2 from "../img/02.png";
import card3 from "../img/03.png";

const Contents = () => {
  return (
    <Carousel style={{ width: "60vh" }}>
      <Carousel.Item>
        <img className="d-block w-100" src={card1} alt="First slide" />
        <Carousel.Caption></Carousel.Caption>
      </Carousel.Item>
      <Carousel.Item>
        <img className="d-block w-100" src={card2} alt="Second slide" />
        <Carousel.Caption></Carousel.Caption>
      </Carousel.Item>
      <Carousel.Item>
        <img className="d-block w-100" src={card3} alt="Third slide" />

        <Carousel.Caption></Carousel.Caption>
      </Carousel.Item>
    </Carousel>
  );
};

export default Contents;
