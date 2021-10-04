import { Container } from "react-bootstrap";
import Header from "./components/Header";
import MainPage from "./pages/MainPage";
import AboutPage from "./pages/AboutPage";
import DownloadPage from "./pages/DownloadPage";
import { BrowserRouter, Switch, Route } from "react-router-dom";
import "./App.css";

function App() {
  return (
    <BrowserRouter>
      <Container className="App">
        <Header></Header>
        <Switch>
          <Route path="/" component={MainPage} />
          <Route path="/" component={AboutPage} />
          <Route path="/" component={DownloadPage} />
        </Switch>
      </Container>
    </BrowserRouter>
  );
}

export default App;
