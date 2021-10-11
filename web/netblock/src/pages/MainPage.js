import React from "react";
import Contents from "../components/Contents";
import MainDownloadButton from "../components/MainDownloadButton";
import { Link } from "react-router-dom";

const MainPage = () => {
  return (
    <div className="mainPage">
      <Contents></Contents>
      <Link to="/download">
        <MainDownloadButton />
      </Link>
    </div>
  );
};

export default MainPage;
