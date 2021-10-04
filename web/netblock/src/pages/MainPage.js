import React from "react";
import { Carousel } from "react-bootstrap";

const MainPage = () => {
  return (
    <div>
      <Carousel>
        <Carousel.Item>
          <img
            className="d-block w-100"
            src="https://www.ui4u.go.kr/depart/img/content/sub03/img_con03030100_01.jpg"
            alt="First slide"
          />
          <Carousel.Caption>
            <h3>First slide label</h3>
            <p>Nulla vitae elit libero, a pharetra augue mollis interdum.</p>
          </Carousel.Caption>
        </Carousel.Item>
        <Carousel.Item>
          <img
            className="d-block w-100"
            src="https://ww.namu.la/s/0826fcb62ab5ffd031695083aa629d99351834b91417a1c9fee4a2a1a4b64bd8287e88163820b02176526fe7006fb51438fbb6f42cb2438497e298e722eac77cde9da7d51d8fa4d36800670013fb43b70d35328129d1f9aec0f9a5ee05ae7fe4"
            alt="Second slide"
          />

          <Carousel.Caption>
            <h3>Second slide label</h3>
            <p>Lorem ipsum dolor sit amet, consectetur adipiscing elit.</p>
          </Carousel.Caption>
        </Carousel.Item>
        <Carousel.Item>
          <img
            className="d-block w-100"
            src="https://cdnweb01.wikitree.co.kr/webdata/editor/202008/29/img_20200829153832_02d1e8ef.webp"
            alt="Third slide"
          />

          <Carousel.Caption>
            <h3>Third slide label</h3>
            <p>
              Praesent commodo cursus magna, vel scelerisque nisl consectetur.
            </p>
          </Carousel.Caption>
        </Carousel.Item>
      </Carousel>
    </div>
  );
};

export default MainPage;
