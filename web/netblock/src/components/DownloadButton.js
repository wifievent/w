import React from "react";
import { Button } from "react-bootstrap";

const DownloadButton = (props) => {
  return (
    <>
      <Button
        className="mt-4"
        variant="dark"
        size="lg"
        onClick={() => alert("Coming Soon")}
      >
        Download
      </Button>
      <h5 className="btnDesc">{props.desc}</h5>
    </>
  );
};

export default DownloadButton;
