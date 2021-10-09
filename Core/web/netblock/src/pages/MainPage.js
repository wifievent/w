import React from "react";
import Contents from "../components/Contents";
import DownloadButton from "../components/DownloadButton";

const MainPage = () => {
  return (
    <div className="mainPage">
      <Contents></Contents>
      <DownloadButton></DownloadButton>
    </div>
  );
};

export default MainPage;
