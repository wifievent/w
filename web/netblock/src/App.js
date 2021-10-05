import { Container } from "react-bootstrap";
import { useState } from "react";
import Header from "./components/Header";
import Footer from "./components/Footer";
import MainPage from "./pages/MainPage";
import AboutPage from "./pages/AboutPage";
import DownloadPage from "./pages/DownloadPage";
import AdModal from "./components/AdModal";
import { BrowserRouter, Switch, Route } from "react-router-dom";
import "./App.css";

function App() {
  const [show, setShow] = useState(true);
  const handleClose = () => setShow(false);
  //const handleShow = () => setShow(true);
  return (
    <BrowserRouter>
      <Container className="App">
        {console.log(show)}
        {show && <AdModal show={show} handleClose={handleClose}></AdModal>}
        <Header />
        <Switch>
          <Route exact path="/" component={MainPage} />
          <Route exact path="/about" component={AboutPage} />
          <Route exact path="/download" component={DownloadPage} />
        </Switch>
        <Footer />
      </Container>
    </BrowserRouter>
  );
}

export default App;
