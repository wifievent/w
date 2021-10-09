import React from "react";
import YouTube from "react-youtube";

class AdYoutube extends React.Component {
  render() {
    const opts = {
      playerVars: {
        // https://developers.google.com/youtube/player_parameters
        autoplay: 1,
      },
    };

    return <YouTube videoId="2tO8WUUYu00" opts={opts} />;
  }
}

export default AdYoutube;
