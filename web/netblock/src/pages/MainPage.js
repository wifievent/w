import React from "react";
import Contents from "../components/Contents";
import MainDownloadButton from "../components/MainDownloadButton";

const MainPage = () => {
  return (
    <div className="mainPage">
      <Contents></Contents>
      <MainDownloadButton />
    </div>
  );
};

export default MainPage;
