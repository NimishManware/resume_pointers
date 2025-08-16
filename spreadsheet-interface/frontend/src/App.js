import { Routes, Route } from "react-router";
import NotFound from "./components/NotFound";
import HomePage from "./pages/HomePage";

function App() {
  return (
    <Routes>
      <Route path="/" element={<HomePage/>}/>
      <Route path="*" element={<NotFound/>}/>
    </Routes>
  );
}

export default App;