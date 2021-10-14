import React from "react";
import DownloadButton from "../components/DownloadButton";

const Download = () => {
  return (
    <>
      <DownloadButton desc="for Windows (64bit)" />
      <DownloadButton desc="for Linux (64bit)" />
    </>
  );
};

export default Download;
